// 
// Animatable.hpp
// Pixels
// 
// Created by Felix Busch on 17.04.2023.
// Copyright © 2023 Felix Busch. All rights reserved.
// 

#pragma once

#include <type_traits> // std::move
#include <memory> // std::unique_ptr
#include <algorithm>

namespace ui
{
	namespace ease
	{
		inline float Linear(float x) { return x; }
	}

	typedef float(*EaseFunction)(float);

	class TimedAnimation
	{
	public:

		inline static constexpr float DefaultDuration = 1.0f;

		explicit constexpr TimedAnimation(EaseFunction ease, float durationInSeconds):
			m_Ease(ease),
			m_DurationInSeconds(durationInSeconds),
			m_ElapsedTimeInSeconds(0.0f),
			m_Forward(true),
			m_Progress(0.0f)
		{
		}

		bool IsDone() const
		{
			return m_ElapsedTimeInSeconds >= m_DurationInSeconds;
		}

		template <typename T>
		void Update(const T& initialValue, const T& targetValue, const T& currentValue, float deltaTime)
		{
			m_ElapsedTimeInSeconds = std::clamp(m_ElapsedTimeInSeconds + deltaTime, 0.0f, m_DurationInSeconds);

			float progress = m_ElapsedTimeInSeconds / m_DurationInSeconds;
			float direction = m_Forward ? 1.0f : -1.0f;
			float x = progress * direction;
			m_Progress = m_Ease(x);
		}

		template <typename T>
		T GetValue(const T& initialValue, const T& targetValue, const T& currentValue) const
		{
			return initialValue + (targetValue - initialValue) * m_Progress;
		}

	private:

		EaseFunction m_Ease;
		float m_DurationInSeconds;
		float m_ElapsedTimeInSeconds;
		bool m_Forward;

		float m_Progress;

	};

	template <typename TAnimation>
	class DelayAnimationDecorator
	{
	public:

		constexpr explicit DelayAnimationDecorator(TAnimation animation, float delayInSeconds):
			m_DelayInSeconds(delayInSeconds),
			m_Animation(std::move(animation))
		{}

		bool IsDone() const
		{
			return m_Animation.IsDone();
		}

		template <typename T>
		void Update(const T& initialValue, const T& targetValue, const T& currentValue, float deltaTime)
		{
			if (m_DelayInSeconds > 0.0f)
			{
				m_DelayInSeconds -= deltaTime;
				return;
			}

			m_Animation.Update(initialValue, targetValue, currentValue, deltaTime);
		}

		template <typename T>
		T GetValue(const T& initialValue, const T& targetValue, const T& currentValue) const
		{
			return m_Animation.GetValue(initialValue, targetValue, currentValue);
		}

	private:

		float m_DelayInSeconds;
		TAnimation m_Animation;

	};

	template <typename TAnimation>
	class AnimationBuilder
	{
	public:

		explicit constexpr AnimationBuilder(TAnimation animation):
			m_Animation(std::move(animation))
		{}

		AnimationBuilder<DelayAnimationDecorator<TAnimation>> Delay(float delayInSeconds) const
		{
			return AnimationBuilder<DelayAnimationDecorator<TAnimation>>(
				DelayAnimationDecorator<TAnimation>(
					m_Animation,
					delayInSeconds
				)
			);
		}

		const TAnimation& GetAnimation() const
		{
			return m_Animation;
		}

	private:

		TAnimation m_Animation;

	};

	constexpr AnimationBuilder<TimedAnimation> EaseLinear(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::Linear,
				durationInSeconds
			)
		);
	}

	template <typename T>
	class Animatable
	{
	private:

		class IAnimationWrapper
		{
		public:

			virtual ~IAnimationWrapper() = default;

			virtual bool IsDone() const = 0;
			virtual void Update(const T& initialValue, const T& targetValue, const T& currentValue, float deltaTime) = 0;
			virtual T GetValue(const T& initialValue, const T& targetValue, const T& currentValue) const = 0;

		};

		template <typename TAnimation>
		class AnimationWrapperImpl : public IAnimationWrapper
		{
		public:

			explicit AnimationWrapperImpl(TAnimation animation):
				m_Animation(std::move(animation))
			{
			}

			virtual bool IsDone() const override
			{
				return m_Animation.IsDone();
			}

			virtual void Update(const T& initialValue, const T& targetValue, const T& currentValue, float deltaTime) override
			{
				m_Animation.Update(initialValue, targetValue, currentValue, deltaTime);
			}

			virtual T GetValue(const T& initialValue, const T& targetValue, const T& currentValue) const override
			{
				return m_Animation.GetValue(initialValue, targetValue, currentValue);
			}

		private:

			TAnimation m_Animation;

		};

	public:

		Animatable(const T& initialValue = T{}):
			m_InitialValue(initialValue),
			m_TargetValue(initialValue),
			m_CurrentValue(initialValue),
			m_Animation(nullptr)
		{
		}

		template <typename TAnimation>
		void WithAnimation(AnimationBuilder<TAnimation> builder, const T& targetValue)
		{
			m_InitialValue = m_CurrentValue;
			m_TargetValue = targetValue;

			m_Animation.reset(new AnimationWrapperImpl<TAnimation>(builder.GetAnimation()));
		}

		void Update(float deltaTime)
		{
			// Exit condition for performance reasons
			if (m_Animation == nullptr)
			{
				return;
			}

			m_Animation->Update(m_InitialValue, m_TargetValue, m_CurrentValue, deltaTime);
			m_CurrentValue = m_Animation->GetValue(m_InitialValue, m_TargetValue, m_CurrentValue);
			
			if (m_Animation->IsDone())
			{
				m_Animation.reset();
			}
		}

		operator const T& () const { return m_CurrentValue; }
		const T* operator -> () const { return &m_CurrentValue; }

		const T& GetInitialValue() const { return m_InitialValue; }
		const T& GetTargetValue() const { return m_TargetValue; }
		const T& GetCurrentValue() const { return m_CurrentValue; }
		const T& Get() const { return m_CurrentValue; }

	private:

		T m_InitialValue;
		T m_TargetValue;
		T m_CurrentValue;

		std::unique_ptr<IAnimationWrapper> m_Animation;

	};

}
