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

		void OnDone()
		{
		}

		template <typename T>
		void Update(const T& initialValue, const T& targetValue, const T& currentValue, float deltaTime)
		{
			m_ElapsedTimeInSeconds = std::clamp(m_ElapsedTimeInSeconds + deltaTime, 0.0f, m_DurationInSeconds);

			float progress = m_ElapsedTimeInSeconds / m_DurationInSeconds;
			float x = m_Forward ? progress : (1.0f - progress);
			m_Progress = m_Ease(x);
		}

		template <typename T>
		T GetValue(const T& initialValue, const T& targetValue, const T& currentValue) const
		{
			return initialValue + (targetValue - initialValue) * m_Progress;
		}

		void Reverse()
		{
			m_Forward = not m_Forward;
		}

		void Repeat()
		{
			m_ElapsedTimeInSeconds = 0.0f;
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
			return not IsWaiting() and m_Animation.IsDone();
		}

		void OnDone()
		{
			m_Animation.OnDone();
		}

		template <typename T>
		void Update(const T& initialValue, const T& targetValue, const T& currentValue, float deltaTime)
		{
			if (IsWaiting())
			{
				m_DelayInSeconds -= deltaTime;
				return;
			}

			m_Animation.Update(initialValue, targetValue, currentValue, deltaTime);
		}

		template <typename T>
		T GetValue(const T& initialValue, const T& targetValue, const T& currentValue) const
		{
			if (IsWaiting())
			{
				return initialValue;
			}

			return m_Animation.GetValue(initialValue, targetValue, currentValue);
		}

		void Repeat()
		{
			m_Animation.Repeat();
		}

		void Reverse()
		{
			m_Animation.Reverse();
		}

	private:

		bool IsWaiting() const
		{
			return m_DelayInSeconds > 0.0f;
		}

		float m_DelayInSeconds;
		TAnimation m_Animation;

	};

	template <typename TAnimation>
	class SpeedAnimationDecorator
	{
	public:

		constexpr explicit SpeedAnimationDecorator(TAnimation animation, float speedFactor) :
			m_SpeedFactor(speedFactor),
			m_Animation(std::move(animation))
		{}

		bool IsDone() const
		{
			return m_Animation.IsDone();
		}

		void OnDone()
		{
			m_Animation.OnDone();
		}

		template <typename T>
		void Update(const T& initialValue, const T& targetValue, const T& currentValue, float deltaTime)
		{
			m_Animation.Update(initialValue, targetValue, currentValue, deltaTime * m_SpeedFactor);
		}

		template <typename T>
		T GetValue(const T& initialValue, const T& targetValue, const T& currentValue) const
		{
			return m_Animation.GetValue(initialValue, targetValue, currentValue);
		}

		void Repeat()
		{
			m_Animation.Repeat();
		}

		void Reverse()
		{
			m_Animation.Reverse();
		}

	private:

		float m_SpeedFactor;
		TAnimation m_Animation;

	};

	template <typename TAnimation, typename TRepeatPredicate, typename TReversePredicate>
	class RepeatAnimationDecorator
	{
	public:

		constexpr explicit RepeatAnimationDecorator(TAnimation animation, TRepeatPredicate repeatPredicate, TReversePredicate reversePredicate):
			m_Animation(std::move(animation)),
			m_RepeatPredicate(std::move(repeatPredicate)),
			m_ReversePredicate(reversePredicate)
		{}

		bool IsDone() const
		{
			return m_Animation.IsDone();
		}

		void OnDone()
		{
			if (m_RepeatPredicate())
			{
				Repeat();

				if (m_ReversePredicate())
				{
					Reverse();
				}
			}
		}

		template <typename T>
		void Update(const T& initialValue, const T& targetValue, const T& currentValue, float deltaTime)
		{
			m_Animation.Update(initialValue, targetValue, currentValue, deltaTime);
		}

		template <typename T>
		T GetValue(const T& initialValue, const T& targetValue, const T& currentValue) const
		{
			return m_Animation.GetValue(initialValue, targetValue, currentValue);
		}

		void Repeat()
		{
			m_Animation.Repeat();
		}

		void Reverse()
		{
			m_Animation.Reverse();
		}

	private:

		TAnimation m_Animation;
		TRepeatPredicate m_RepeatPredicate;
		TReversePredicate m_ReversePredicate;

	};

	struct Predicate
	{
		inline static constexpr auto Always = [] { return true; };
		inline static constexpr auto Never = [] { return false; };
		
		typedef decltype(Always) AlwaysType;
		typedef decltype(Never) NeverType;
	};

	template <typename TAnimation>
	class AnimationBuilder;

	template <typename TAnimation>
	AnimationBuilder(TAnimation) -> AnimationBuilder<TAnimation>;

	template <typename TAnimation>
	class AnimationBuilder
	{
	public:

		explicit constexpr AnimationBuilder(TAnimation animation):
			m_Animation(std::move(animation))
		{}

		auto Delay(float delayInSeconds) const
		{
			return AnimationBuilder<DelayAnimationDecorator<TAnimation>>(
				DelayAnimationDecorator<TAnimation>(
					m_Animation,
					delayInSeconds
				)
			);
		}

		auto Speed(float speedFactor) const
		{
			return AnimationBuilder<SpeedAnimationDecorator<TAnimation>>(
				SpeedAnimationDecorator<TAnimation>(
					m_Animation,
					speedFactor
				)
			);
		}

		template <
			typename TRepeatPredicate = Predicate::AlwaysType,
			typename TReversePredicate = Predicate::NeverType
		>
		auto Repeat(
			TRepeatPredicate repeatPredicate = Predicate::Always,
			TReversePredicate reversePredicate = Predicate::Never
		) const
		{
			return AnimationBuilder<RepeatAnimationDecorator<TAnimation, TRepeatPredicate, TReversePredicate>>(
				RepeatAnimationDecorator<TAnimation, TRepeatPredicate, TReversePredicate>(
					m_Animation,
					std::move(repeatPredicate),
					std::move(reversePredicate)
				)
			);
		}

		template <typename TReversePredicate = Predicate::NeverType>
		auto RepeatFor(
			std::size_t count,
			TReversePredicate reversePredicate = Predicate::Never
		) const
		{
			auto repeatPredicate = [count, i = (std::size_t)0]() mutable
			{
				return ++i < count;
			};

			return AnimationBuilder<RepeatAnimationDecorator<TAnimation, decltype(repeatPredicate), TReversePredicate>>(
				RepeatAnimationDecorator<TAnimation, decltype(repeatPredicate), TReversePredicate>(
					m_Animation,
					std::move(repeatPredicate),
					std::move(reversePredicate)
				)
			);
		}

		template <typename TRepeatPredicate = Predicate::AlwaysType>
		auto ReverseFor(
			std::size_t count,
			TRepeatPredicate repeatPredicate = Predicate::Always
		) const
		{
			auto reversePredicate = [count, i = (std::size_t)0]() mutable
			{
				return i++ < count;
			};

			return AnimationBuilder<RepeatAnimationDecorator<TAnimation, TRepeatPredicate, decltype(reversePredicate)>>(
				RepeatAnimationDecorator<TAnimation, TRepeatPredicate, decltype(reversePredicate)>(
					m_Animation,
					std::move(repeatPredicate),
					std::move(reversePredicate)
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

	class InstantAnimation
	{
	public:

		bool IsDone() const
		{
			return true;
		}

		void OnDone() const
		{
		}

		template <typename T>
		void Update(const T& initialValue, const T& targetValue, const T& currentValue, float deltaTime)
		{
		}

		template <typename T>
		T GetValue(const T& initialValue, const T& targetValue, const T& currentValue) const
		{
			return targetValue;
		}

		void Repeat() const
		{
		}

		void Reverse() const
		{
		}

	};

	constexpr AnimationBuilder<InstantAnimation> Instant()
	{
		return AnimationBuilder(InstantAnimation());
	}

	class NeverAnimation
	{
	public:

		bool IsDone() const
		{
			return false;
		}

		void OnDone() const
		{
		}

		template <typename T>
		void Update(const T& initialValue, const T& targetValue, const T& currentValue, float deltaTime)
		{
		}

		template <typename T>
		T GetValue(const T& initialValue, const T& targetValue, const T& currentValue) const
		{
			return initialValue;
		}

		void Repeat() const
		{
		}

		void Reverse() const
		{
		}

	};

	constexpr AnimationBuilder<NeverAnimation> Never()
	{
		return AnimationBuilder(NeverAnimation());
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
			virtual void OnDone() = 0;
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

			virtual void OnDone() override
			{
				m_Animation.OnDone();
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
				m_Animation->OnDone();

				if (m_Animation->IsDone())
				{
					m_Animation.reset();
				}
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
