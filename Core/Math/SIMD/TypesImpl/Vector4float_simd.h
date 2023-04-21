#pragma once

#include "Math/SIMD/Platform.h"


#ifdef MATH_SSE_2

namespace Math
{
#define Size4 4

	template <>
	class Vector <float, Size4>
	{
	public:
		using iterator = VectorIterator<float, Size4>;
		using const_iterator = VectorConstIterator<float, Size4>;

// -------------Constructors-------------------------------------
	public:
		inline Vector() = default;

		explicit inline Vector(float scalar)
			: _data(_mm_set_ps1(scalar)) {}
		

		constexpr Vector(__m128 data)
			: _data(data) {}


		template<typename X, typename Y, typename Z, typename W>
		inline Vector(X _x, Y _y, Z _z, W _w)
			: _data(_mm_set_ps(static_cast<float>(_w), 
				static_cast<float>(_z), 
				static_cast<float>(_y), 
				static_cast<float>(_x))) {}


		constexpr Vector(const Vector<float, Size4>& other)
			: _data(other._data) {}


		constexpr Vector& operator=(const Vector<float, Size4>& other)
		{
			if (other != *this)
				_data = other._data;

			return *this;
		}


		template <typename U>
		inline Vector<float, Size4>(const Vector<U, Size4>& other)
			: _data(_mm_set_ps(static_cast<float>(other.w),
				static_cast<float>(other.z),
				static_cast<float>(other.y),
				static_cast<float>(other.x))) {}


		template <typename U>
		inline Vector<float, Size4>& operator=(const Vector<U, Size4>& other)
		{
			_data = other._data;
			return *this;
		}


		template <typename U>
		inline Vector<float, Size4>(const Vector<U, 2>& other)
			: _data(_mm_set_ps(1.f,
				1.f,
				static_cast<float>(other.y),
				static_cast<float>(other.x))) {}		


		template <typename U>
		inline Vector<float, Size4>& operator=(const Vector<U, 2>& other)
		{
			_data = _mm_set_ps(1.f,
				1.f,
				static_cast<float>(other.y),
				static_cast<float>(other.x));

			return *this;
		}


		template <typename X, typename Y, typename Z>
		inline Vector<float, Size4>(const Vector<X, 2>& _xy, Y _z, Z _w)
			: _data(_mm_set_ps(static_cast<float>(_w),
				static_cast<float>(_z),
				static_cast<float>(_xy.y),
				static_cast<float>(_xy.x))) {}


		template <typename X, typename Y, typename Z>
		inline Vector<float, Size4>(X _x, Y _y, const Vector<Z, 2>& _zw)
			: _data(_mm_set_ps(static_cast<float>(_zw.y),
				static_cast<float>(_zw.x),
				static_cast<float>(_y),
				static_cast<float>(_x))) {}


		template <typename X, typename Y, typename Z>
		inline Vector<float, Size4>(X _x, const Vector<Y, 2>& _yz, Z _w)
			: _data(_mm_set_ps(static_cast<float>(_w),
				static_cast<float>(_yz.y),
				static_cast<float>(_yz.x),
				static_cast<float>(_x))) {}


		template <typename X, typename Y>
		inline Vector<float, Size4>(const Vector<X, 2>& _xy, const Vector<Y, 2>& _zw)
			: _data(_mm_set_ps(static_cast<float>(_xy.x),
				static_cast<float>(_xy.y),
				static_cast<float>(_zw.x),
				static_cast<float>(_zw.y))) {}


		template <typename U>
		inline Vector<float, Size4>(const Vector<U, 3>& other)
		{
			_data = _mm_set_ps(1.f,
				static_cast<float>(other.z),
				static_cast<float>(other.y),
				static_cast<float>(other.x));
		}


		template <typename U>
		inline Vector<float, Size4>& operator=(const Vector<U, 3>& other)
		{
			_data = _mm_set_ps(1.f,
				static_cast<float>(other.z),
				static_cast<float>(other.y),
				static_cast<float>(other.x));

			return *this;
		}

		template <typename X, typename Y>
		inline Vector<float, Size4>(const Vector<X, 3>& _xyz, Y _w)
			: _data(_mm_set_ps(static_cast<float>(_w),
				static_cast<float>(_xyz.z),
				static_cast<float>(_xyz.y),
				static_cast<float>(_xyz.x))) {}


		template <typename X, typename Y>
		inline Vector<float, Size4>(X _x, const Vector<Y, 3>& _yzw)
			: _data(_mm_set_ps(static_cast<float>(_yzw.z),
				static_cast<float>(_yzw.y),
				static_cast<float>(_yzw.x),
				static_cast<float>(_x))) {}

// -------------Public Methods-------------------------------------
	public:
		constexpr uint32 Size() const
		{
			return Size4;
		}

		constexpr float* Data()
		{
			return &x;
		}

		constexpr const float* Data() const
		{
			return &x;
		}

		constexpr iterator begin()
		{
			return iterator(&x, 0);
		}

		constexpr iterator end()
		{
			return iterator(&x, Size4);
		}

		constexpr const_iterator cbegin() const
		{
			return const_iterator(&x, 0);
		}

		constexpr const_iterator cend() const
		{
			return const_iterator(&x, Size4);
		}

		inline void Fill(float value)
		{
			_data = _mm_set_ps1(value);
		}

		inline Vector& Apply(float(*func)(float))
		{
			_data = _mm_set_ps(func(w), func(z), func(y), func(x));
			return *this;
		}

		inline float SqrLength() const
		{
			return Math::Dot(*this, *this);
		}

		inline float Length() const
		{
			return Math::Sqrt(SqrLength());
		}

		inline Vector& Normalize()
		{
			float length = Length();
			return length == 0 ? *this : *this /= length;
		}

		inline Vector GetNormalized() const
		{
			float length = Length();
			return length == 0 ? Vector(*this) : *this / length;
		}
	
// -------------Operators-------------------------------------
	public:
		constexpr const float& operator[](uint32 idx) const
		{
			ASSERT(idx < Size4);
			return *(&x + idx);
		}

		constexpr float& operator[](uint32 idx)
		{
			ASSERT(idx < Size4);
			return *(&x + idx);
		}

		inline Vector& operator+=(const Vector& other)
		{
			_data = _mm_add_ps(_data, other._data);
			return *this;
		}

		inline Vector& operator-=(const Vector& other)
		{
			_data = _mm_sub_ps(_data, other._data);
			return *this;
		}

		inline Vector& operator*=(const Vector& other)
		{
			_data = _mm_mul_ps(_data, other._data);
			return *this;
		}

		inline Vector& operator/=(const Vector& other)
		{
			_data = _mm_div_ps(_data, other._data);
			return *this;
		}

		inline Vector& operator+=(float scalar)
		{
			_data = _mm_add_ps(_data, _mm_set_ps1(scalar));
			return *this;
		}

		inline Vector& operator-=(float scalar)
		{
			_data = _mm_sub_ps(_data, _mm_set_ps1(scalar));
			return *this;
		}

		inline Vector& operator*=(float scalar)
		{
			_data = _mm_mul_ps(_data, _mm_set_ps1(scalar));
			return *this;
		}

		inline Vector& operator/=(float scalar)
		{
			_data = _mm_div_ps(_data, _mm_set_ps1(scalar));
			return *this;
		}

		inline Vector operator+(const Vector& other) const
		{
			return Vector(_mm_add_ps(_data, other._data));
		}

		inline Vector operator-(const Vector& other) const
		{
			return Vector(_mm_sub_ps(_data, other._data));
		}

		inline Vector operator*(const Vector& other) const
		{
			return Vector(_mm_mul_ps(_data, other._data));
		}

		inline Vector operator/(const Vector& other) const
		{
			return Vector(_mm_div_ps(_data, other._data));
		}

		inline Vector operator+(float scalar) const
		{
			return Vector(_mm_add_ps(_data, _mm_set_ps1(scalar)));
		}

		inline Vector operator-(float scalar) const
		{
			return Vector(_mm_sub_ps(_data, _mm_set_ps1(scalar)));
		}

		inline Vector operator*(float scalar) const
		{
			return Vector(_mm_mul_ps(_data, _mm_set_ps1(scalar)));
		}

		inline Vector operator/(float scalar) const
		{
			return Vector(_mm_div_ps(_data, _mm_set_ps1(scalar)));
		}

		inline Vector operator-() const
		{
			return Vector(_mm_set_ps(-w, -z, -y, -x));
		}

		constexpr bool operator==(const Vector& other) const
		{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}

		constexpr bool operator!=(const Vector& other) const
		{
			return x != other.x || y != other.y || z != other.z || w != other.w;
		}

	public:
		union
		{
			__m128 _data;
			struct
			{
				float x, y, z, w;
			};
		};
	};

#undef Size4
}


#endif // ATN_SSE_2
