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
#include <algorithm> // std::clamp
#include <numbers> // std::numbers::pi_v<float>
#include <vector> // std::vector

namespace ui
{
	namespace ease
	{
		inline float Linear(float x)
		{
			return x;
		}
		inline float InSine(float x)
		{
			return 1.0f - std::cos((x * std::numbers::pi_v<float>) / 2.0f);
		}
		inline float OutSine(float x)
		{
			return std::sin((x * std::numbers::pi_v<float>) / 2.0f);
		}
		inline float InOutSine(float x)
		{
			return -(std::cos(std::numbers::pi_v<float> * x) - 1.0f) / 2.0f;
		}
		inline float InCubic(float x)
		{
			return x * x * x;
		}
		inline float OutCubic(float x)
		{
			return 1.0f - std::pow(1.0f - x, 3.0f);
		}
		inline float InOutCubic(float x)
		{
			return x < 0.5f ? 4.0f * x * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 3.0f) / 2.0f;
		}
		inline float InQuint(float x)
		{
			return x * x * x * x * x;
		}
		inline float OutQuint(float x)
		{
			return 1.0f - std::pow(1.0f - x, 5.0f);
		}
		inline float InOutQuint(float x)
		{
			return x < 0.5f ? 16.0f * x * x * x * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 5.0f) / 2.0f;
		}
		inline float InCirc(float x)
		{
			return 1.0f - std::sqrt(1.0f - std::pow(x, 2.0f));
		}
		inline float OutCirc(float x)
		{
			return std::sqrt(1.0f - std::pow(x - 1.0f, 2.0f));
		}
		inline float InOutCirc(float x)
		{
			return x < 0.5f
				? (1.0f - std::sqrt(1.0f - std::pow(2.0f * x, 2.0f))) / 2.0f
				: (std::sqrt(1.0f - std::pow(-2.0f * x + 2.0f, 2.0f)) + 1.0f) / 2.0f;
		}
		inline float InElastic(float x)
		{
			static constexpr float c4 = (2.0f * std::numbers::pi_v<float>) / 3.0f;

			return x == 0.0f
				? 0.0f
				: x == 1.0f
				? 1.0f
				: -std::pow(2.0f, 10.0f * x - 10.0f) * std::sin((x * 10.0f - 10.75f) * c4);
		}
		inline float OutElastic(float x)
		{
			static constexpr float c4 = (2.0f * std::numbers::pi_v<float>) / 3.0f;

			return x == 0.0f
				? 0.0f
				: x == 1.0f
				? 1.0f
				: pow(2.0f, -10.0f * x) * sin((x * 10.0f - 0.75f) * c4) + 1.0f;
		}
		inline float InOutElastic(float x)
		{
			static constexpr float c5 = (2.0f * std::numbers::pi_v<float>) / 4.5f;

			return x == 0.0f
				? 0.0f
				: x == 1.0f
				? 1.0f
				: x < 0.5f
				? -(std::pow(2.0f, 20.0f * x - 10.0f) * std::sin((20.0f * x - 11.125f) * c5)) / 2.0f
				: (std::pow(2.0f, -20.0f * x + 10.0f) * std::sin((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
		}
		inline float InQuad(float x)
		{
			return x * x;
		}
		inline float OutQuad(float x)
		{
			return 1.0f - (1.0f - x) * (1.0f - x);
		}
		inline float InOutQuad(float x)
		{
			return x < 0.5f ? 2.0f * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 2.0f) / 2.0f;
		}
		inline float InQuart(float x)
		{
			return x * x * x * x;
		}
		inline float OutQuart(float x)
		{
			return 1.0f - std::pow(1.0f - x, 4.0f);
		}
		inline float InOutQuart(float x)
		{
			return x < 0.5f ? 8.0f * x * x * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 4.0f) / 2.0f;
		}
		inline float InExpo(float x)
		{
			return x == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * x - 10.0f);
		}
		inline float OutExpo(float x)
		{
			return x == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * x);
		}
		inline float InOutExpo(float x)
		{
			return x == 0.0f
				? 0.0f
				: x == 1.0f
				? 1.0f
				: x < 0.5f ? pow(2.0f, 20.0f * x - 10.0f) / 2.0f
				: (2.0f - pow(2.0f, -20.0f * x + 10.0f)) / 2.0f;
		}
		inline float InBack(float x)
		{
			static constexpr float c1 = 1.70158f;
			static constexpr float c3 = c1 + 1.0f;

			return c3 * x * x * x - c1 * x * x;
		}
		inline float OutBack(float x)
		{
			static constexpr float c1 = 1.70158f;
			static constexpr float c3 = c1 + 1.0f;
			return 1.0f + c3 * std::pow(x - 1.0f, 3.0f) + c1 * std::pow(x - 1.0f, 2.0f);
		}
		inline float InOutBack(float x)
		{
			static constexpr float c1 = 1.70158f;
			static constexpr float c2 = c1 * 1.525f;

			return x < 0.5f
				? (std::pow(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) / 2.0f
				: (std::pow(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
		}
		inline float OutBounce(float x)
		{
			static constexpr float n1 = 7.5625f;
			static constexpr float d1 = 2.75f;

			if (x < 1.0f / d1) {
				return n1 * x * x;
			}

			if (x < 2.0f / d1) {
				return n1 * (x -= 1.5f / d1) * x + 0.75f;
			}

			if (x < 2.5f / d1) {
				return n1 * (x -= 2.25f / d1) * x + 0.9375f;
			}
			
			return n1 * (x -= 2.625f / d1) * x + 0.984375f;
		}
		inline float InBounce(float x)
		{
			return 1.0f - OutBounce(1.0f - x);
		}
		inline float InOutBounce(float x)
		{
			return x < 0.5f
				? (1.0f - OutBounce(1.0f - 2.0f * x)) / 2.0f
				: (1.0f + OutBounce(2.0f * x - 1.0f)) / 2.0f;
		}
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

	class InstantAnimation
	{
	public:

		constexpr InstantAnimation() = default;

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

	class NeverAnimation
	{
	public:

		constexpr NeverAnimation() = default;

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

	class TimingCurveAnimation
	{
	public:

		template <std::ranges::range TValues, std::ranges::range TFunctions>
		constexpr explicit TimingCurveAnimation(float durationInSeconds, TValues values, TFunctions functions):
			m_Values(std::ranges::begin(values), std::ranges::end(values)),
			m_Functions(std::ranges::begin(functions), std::ranges::end(functions)),
			m_DurationInSeconds(durationInSeconds),
			m_Value(m_Values.empty() ? 0.0f : m_Values.front())
		{
		}

		bool IsDone() const
		{
			return m_ValueIndex >= m_Values.size();
		}

		void OnDone() const
		{
		}

		template <typename T>
		void Update(const T& initialValue, const T& targetValue, const T& currentValue, float deltaTime)
		{
			float durationPerValue = m_DurationInSeconds / static_cast<float>(m_Values.size());
			m_ElapsedTimeInSeconds = std::clamp(m_ElapsedTimeInSeconds + deltaTime, 0.0f, durationPerValue);

			float progress = m_ElapsedTimeInSeconds / durationPerValue;
			EaseFunction ease = GetFunction();
			float previousTarget = GetPreviousTargetValue();
			float target = GetTargetValue();
			m_Value = std::lerp(previousTarget, target, ease(progress));

			if (m_ElapsedTimeInSeconds >= durationPerValue)
			{
				++m_ValueIndex;
				m_ElapsedTimeInSeconds = 0.0f;
			}
		}

		template <typename T>
		T GetValue(const T& initialValue, const T& targetValue, const T& currentValue) const
		{
			return initialValue + (targetValue - initialValue) * m_Value;
		}

		void Repeat()
		{
			m_ValueIndex = 1;
		}

		void Reverse()
		{
			std::ranges::reverse(m_Values);
			std::ranges::reverse(m_Functions);
		}

	private:

		float GetPreviousTargetValue() const
		{
			if (m_Values.empty())
				return 0.0f;

			return m_Values[std::min(m_ValueIndex - 1, m_Values.size() - 1)];
		}

		float GetTargetValue() const
		{
			if (m_Values.empty())
				return 0.0f;

			return m_Values[std::min(m_ValueIndex, m_Values.size() - 1)];
		}

		EaseFunction GetFunction() const
		{
			if (m_Functions.empty())
				return &ease::Linear;

			return m_Functions[std::min(m_ValueIndex - 1, m_Functions.size() - 1)];
		}

		std::vector<float> m_Values;
		std::vector<EaseFunction> m_Functions;

		float m_ElapsedTimeInSeconds = 0.0f;
		float m_DurationInSeconds;
		std::size_t m_ValueIndex = 1;
		float m_Value;

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
	constexpr AnimationBuilder<TimedAnimation> EaseInSine(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InSine,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseOutSine(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::OutSine,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInOutSine(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InOutSine,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInCubic(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InCubic,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseOutCubic(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::OutCubic,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInOutCubic(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InOutCubic,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInQuint(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InQuint,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseOutQuint(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::OutQuint,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInOutQuint(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InOutQuint,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInCirc(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InCirc,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseOutCirc(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::OutCirc,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInOutCirc(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InOutCirc,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInElastic(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InElastic,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseOutElastic(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::OutElastic,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInOutElastic(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InOutElastic,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInQuad(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InQuad,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseOutQuad(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::OutQuad,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInOutQuad(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InOutQuad,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInQuart(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InQuart,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseOutQuart(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::OutQuart,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInOutQuart(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InOutQuart,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInExpo(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InExpo,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseOutExpo(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::OutExpo,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInOutExpo(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InOutExpo,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInBack(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InBack,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseOutBack(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::OutBack,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInOutBack(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InOutBack,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInBounce(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InBounce,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseOutBounce(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::OutBounce,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<TimedAnimation> EaseInOutBounce(float durationInSeconds = TimedAnimation::DefaultDuration)
	{
		return AnimationBuilder(
			TimedAnimation(
				&ease::InOutBounce,
				durationInSeconds
			)
		);
	}
	constexpr AnimationBuilder<InstantAnimation> Instant()
	{
		return AnimationBuilder(InstantAnimation());
	}
	constexpr AnimationBuilder<NeverAnimation> Never()
	{
		return AnimationBuilder(NeverAnimation());
	}

	template <std::ranges::range TValues, std::ranges::range TFunctions>
	constexpr AnimationBuilder<TimingCurveAnimation> TimingCurve(
		float durationInSeconds,
		TValues values,
		TFunctions functions
	)
	{
		return AnimationBuilder<TimingCurveAnimation>(
			TimingCurveAnimation(
				durationInSeconds,
				std::move(values),
				std::move(functions)
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

		constexpr Animatable(const T& initialValue = T{}):
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

		void WithoutAnimation(const T& targetValue)
		{
			m_InitialValue = targetValue;
			m_TargetValue = targetValue;
			m_CurrentValue = targetValue;
			m_Animation.reset();
		}

		void Update(float deltaTime)
		{
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