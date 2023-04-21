#pragma once

#include "Vector.h"



namespace Math
{
#define Size3 3

	template <typename T>
	class Vector<T, Size3>
	{
	public:
		using iterator = VectorIterator<T, Size3>;
		using const_iterator = VectorConstIterator<T, Size3>;

// -------------Constructors-------------------------------------
	public:
		constexpr Vector() = default;

		explicit constexpr Vector(T scalar)
			: x(scalar),
			y(scalar),
			z(scalar) {}


		template<typename X, typename Y, typename Z>
		constexpr Vector(X _x, Y _y, Z _z)
			: x(static_cast<T>(_x)),
			y(static_cast<T>(_y)),
			z(static_cast<T>(_z)) {}


		template <typename U>
		constexpr Vector<T, Size3>(const Vector<U, Size3>& other)
			: x(static_cast<T>(other.x)),
			y(static_cast<T>(other.y)),
			z(static_cast<T>(other.z)) {}


		template <typename U>
		constexpr Vector<T, Size3>& operator=(const Vector<U, Size3>& other)
		{
			x = static_cast<T>(other.x);
			y = static_cast<T>(other.y);
			z = static_cast<T>(other.z);

			return *this;
		}


		template <typename X, typename Y>
		constexpr Vector<T, Size3>(const Vector<X, 2>& _xy, Y _z)
			: x(static_cast<T>(_xy.x)),
			y(static_cast<T>(_xy.y)),
			z(static_cast<T>(_z)) {}


		template <typename X, typename Y>
		constexpr Vector<T, Size3>(X _x, const Vector<Y, 2>& _yz)
			: x(static_cast<T>(_x)),
			y(static_cast<T>(_yz.x)),
			z(static_cast<T>(_yz.y)) {}


		template <typename U>
		constexpr Vector<T, Size3>(const Vector<U, 2>& _xy)
			: x(static_cast<T>(_xy.x)),
			y(static_cast<T>(_xy.y)),
			z(static_cast<T>(1)) {}


		template <typename U>
		constexpr Vector<T, Size3>& operator=(const Vector<U, 2>& _xy)
		{
			x = static_cast<T>(_xy.x);
			y = static_cast<T>(_xy.y);
			z = static_cast<T>(1);

			return *this;
		}


		template <typename U>
		constexpr Vector<T, Size3>(const Vector<U, 4>& _xyz)
			: x(static_cast<T>(_xyz.x)),
			y(static_cast<T>(_xyz.y)),
			z(static_cast<T>(_xyz.z)) {}


		template <typename U>
		constexpr Vector<T, Size3>& operator=(const Vector<U, 4>& _xyz)
		{
			x = static_cast<T>(_xyz.x);
			y = static_cast<T>(_xyz.y);
			z = static_cast<T>(_xyz.z);

			return *this;
		}

// -------------Public Methods-------------------------------------
	public:
		constexpr uint32_t Size() const
		{
			return Size3;
		}

		constexpr T* Data()
		{
			return &x;
		}

		constexpr const T* Data() const
		{
			return &x;
		}

		constexpr iterator begin()
		{
			return iterator(&x, 0);
		}

		constexpr iterator end()
		{
			return iterator(&x, Size3);
		}

		constexpr const_iterator cbegin() const
		{
			return const_iterator(&x, 0);
		}

		constexpr const_iterator cend() const
		{
			return const_iterator(&x, Size3);
		}

		constexpr void Fill(T value)
		{
			x = value;
			y = value;
			z = value;
		}

		constexpr Vector& Apply(T(*func)(T))
		{
			x = func(x);
			y = func(y);
			z = func(z);
			return *this;
		}

		constexpr T SqrLength() const
		{
			return Math::Dot(*this, *this);
		}

		constexpr float Length() const
		{
			return Math::Sqrt(SqrLength());
		}

		constexpr Vector& Normalize()
		{
			float length = Length();
			return length == 0 ? *this : *this /= static_cast<T>(length);
		}

		constexpr Vector GetNormalized() const
		{
			float length = Length();
			return length == 0 ? Vector(*this) : Vector(*this) /= static_cast<T>(length);
		}

// -------------Operators-------------------------------------
	public:
		constexpr const T& operator[](uint32_t idx) const
		{
			ASSERT(idx < Size3);
			return *(&x + idx);
		}

		constexpr T& operator[](uint32_t idx)
		{
			ASSERT(idx < Size3);
			return *(&x + idx);
		}

		constexpr Vector& operator+=(const Vector& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		constexpr Vector& operator-=(const Vector& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		constexpr Vector& operator*=(const Vector& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		constexpr Vector& operator/=(const Vector& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}

		constexpr Vector& operator+=(T scalar)
		{
			x += scalar;
			y += scalar;
			z += scalar;
			return *this;
		}

		constexpr Vector& operator-=(T scalar)
		{
			x -= scalar;
			y -= scalar;
			z -= scalar;
			return *this;
		}

		constexpr Vector& operator*=(T scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		constexpr Vector& operator/=(T scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		constexpr Vector operator+(const Vector& other) const
		{
			return Vector(x + other.x, y + other.y, z + other.z);
		}

		constexpr Vector operator-(const Vector& other) const
		{
			return Vector(x - other.x, y - other.y, z - other.z);
		}

		constexpr Vector operator*(const Vector& other) const
		{
			return Vector(x * other.x, y * other.y, z * other.z);
		}

		constexpr Vector operator/(const Vector& other) const
		{
			return Vector(x / other.x, y / other.y, z / other.z);
		}

		constexpr Vector operator+(T scalar) const
		{
			return Vector(x + scalar, y + scalar, z + scalar);
		}

		constexpr Vector operator-(T scalar) const
		{
			return Vector(x - scalar, y - scalar, z - scalar);
		}

		constexpr Vector operator*(T scalar) const
		{
			return Vector(x * scalar, y * scalar, z * scalar);
		}

		constexpr Vector operator/(T scalar) const
		{
			return Vector(x / scalar, y / scalar, z / scalar);
		}

		constexpr Vector operator-() const
		{
			return Vector(-x, -y, -z);
		}

		constexpr bool operator==(const Vector& other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		constexpr bool operator!=(const Vector& other) const
		{
			return x != other.x || y != other.y || z != other.z;
		}

// -------------Static Methods-------------------------------------
	public:
		static constexpr Vector Up()
		{
			return Vector(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
		}

		static constexpr Vector Down()
		{
			return Vector(static_cast<T>(0), static_cast<T>(-1), static_cast<T>(0));
		}

		static constexpr Vector Left()
		{
			return Vector(static_cast<T>(-1), static_cast<T>(0), static_cast<T>(0));
		}

		static constexpr Vector Right()
		{
			return Vector(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
		}

		static constexpr Vector Forward()
		{
			return Vector(static_cast<T>(0), static_cast<T>(0), static_cast<T>(-1));
		}

		static constexpr Vector Back()
		{
			return Vector(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
		}

	public:
		T x, y, z;
	};

#undef Size3



// -------------Relative Functions-------------------------------------

	template <typename T>
	constexpr T Dot(const Vector<T, 3>& left, const Vector<T, 3>& right)
	{
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}

	template <typename T>
	constexpr Vector<T, 3> Cross(const Vector<T, 3>& left, const Vector<T, 3>& right)
	{
		Vector<T, 3> out;
		out.x = left.y * right.z - left.z * right.y;
		out.y = -left.x * right.z + left.z * right.x;
		out.z = left.x * right.y - left.y * right.x;
		return out;
	}

	// @param normal: Expected to be normalized
	template <typename T>
	constexpr Vector<T, 3> Reflect(
		const Vector<T, 3>& direction, const Vector<T, 3>& normal)
	{
		return direction - 2 * normal * Math::Dot(direction, normal);
	}
}


#ifdef APP_SIMD
	#include "Math/SIMD/TypesImpl/Vector4float_simd.h"
	#include "Math/SIMD/TypesImpl/VectorRelational_simd.h"
#endif
