#pragma once

#include "Core/Core.h"

#include <chrono>


namespace Core
{
	class Time
	{
	public:
		using duration_type = std::chrono::duration<double, std::micro>;

	public:
		Time(float seconds = 0)
			: m_Time(std::chrono::duration<double>(seconds)) {}

		template <typename Rep, typename Period>
		Time(std::chrono::duration<Rep, Period> duration)
			: m_Time(std::chrono::duration_cast<duration_type>(duration)) {}

		inline operator double() const
		{
			return m_Time.count();
		}

		inline double AsSeconds() const
		{
			return m_Time.count() * 0.000001;
		}

		inline double AsMilliseconds() const
		{
			return m_Time.count() * 0.001;
		}

		inline double AsMicroseconds() const
		{
			return m_Time.count();
		}
		
		inline Time& operator+=(Time other)
		{
			m_Time += other.m_Time;
			return *this;;
		}

		inline Time& operator-=(Time other)
		{
			m_Time -= other.m_Time;
			return *this;;
		}

		inline Time operator+(Time other) const
		{
			return Time(m_Time + other.m_Time);
		}

		inline Time operator-(Time other) const
		{
			return Time(m_Time - other.m_Time);
		}

		inline bool operator==(Time other) const
		{
			return m_Time == other.m_Time;
		}

		inline bool operator!=(Time other) const
		{
			return m_Time != other.m_Time;
		}

		inline bool operator>(Time other) const
		{
			return m_Time > other.m_Time;
		}

		inline bool operator<(Time other) const
		{
			return m_Time < other.m_Time;
		}

		inline bool operator>=(Time other) const
		{
			return m_Time >= other.m_Time;
		}

		inline bool operator<=(Time other) const
		{
			return m_Time <= other.m_Time;
		}

	public:
		static inline Time Seconds(double seconds)
		{
			return Time(seconds);
		}

		static inline Time Milliseconds(double milliseconds)
		{
			return Time(std::chrono::duration<double, std::milli>(milliseconds));
		}

		static inline Time Microseconds(double microseconds)
		{
			return Time(std::chrono::duration<double, std::micro>(microseconds));
		}

	private:
		duration_type m_Time;
	};


	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		inline void Reset()
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		inline Time ElapsedTime()
		{
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - m_Start;
			return Time(diff);
		}

	private:
		std::chrono::high_resolution_clock::time_point m_Start;
	};
}
