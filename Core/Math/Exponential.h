#pragma once

#include <cmath>


namespace Math
{
	template <typename T>
	inline T Sqrt(T scalar)
	{
		return static_cast<T>(std::sqrt(scalar));
	}

	template <typename T>
	inline T Cbrt(T scalar)
	{
		return static_cast<T>(std::cbrt(scalar));
	}

	template <typename T>
	inline T ExpE(T scalar)
	{
		return std::exp(scalar);
	}

	template <typename T>
	inline T Exp2(T scalar)
	{
		return std::exp2(scalar);
	}

	template <typename T>
	inline T LogE(T scalar)
	{
		return std::log(scalar);
	}

	template <typename T>
	inline T Log2(T scalar)
	{
		return std::log2(scalar);
	}

	template <typename T>
	inline T Log10(T scalar)
	{
		return std::log10(scalar);
	}

	template <typename T>
	inline T Pow(T scalar, T exp)
	{
		return std::pow(scalar, exp);
	}
}
