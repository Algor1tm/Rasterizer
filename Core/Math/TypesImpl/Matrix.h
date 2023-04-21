#pragma once

#include "Core/Core.h"
#include "Vector.h"

#include "Math/Common.h"

#include <sstream>


namespace Math
{
	using Core::uint32;

	// Column - size of columns
	// Row - size of rows
	template <typename T, uint32_t Column, uint32_t Row>
	class Matrix
	{
	public:
		using RowType = Vector<T, Row>;
		using ColumnType = Vector<T, Column>;

	public:
		using iterator = VectorIterator<Vector<T, Row>, Column>;
		using const_iterator = VectorConstIterator<Vector<T, Row>, Column>;

// -------------Constructors-------------------------------------
	public:
		constexpr Matrix() = default;

		explicit constexpr Matrix(T scalar)
		{
			Fill(scalar);
		}

		constexpr Matrix(const std::initializer_list<RowType>& values)
		{
			ASSERT(values.size() == Column);
			uint32 idx = 0;
			for (auto& row : values)
			{
				m_Array[idx] = row;
				++idx;
			}
		}

		constexpr Matrix(const RowType& vec)
		{
			static_assert(Column == 1, "Cannot initialize matrix with vector");
			m_Array[0] = vec;
		}

		template <typename U>
		constexpr Matrix(const Matrix<U, Column, Row>& other)
		{
			for (uint32 i = 0; i < Column; ++i)
				m_Array[i] = static_cast<T>(other[i]);
		}


		template <typename U>
		constexpr Matrix<T, Column, Row>& operator=(const Matrix<U, Column, Row>& other)
		{
			for (uint32 i = 0; i < Column; ++i)
				m_Array[i] = static_cast<T>(other[i]);
		}

// -------------Type Casts-------------------------------------
	public:
		constexpr explicit operator Vector<T, Row>() const
		{
			static_assert(Column == 1, "Cannot convert matrix to vector");
			return m_Array[0];
		}


// -------------Public Methods-------------------------------------
	public:
		constexpr T* Data()
		{
			return &(m_Array[0][0]);
		}

		constexpr const T* Data() const
		{
			return &(m_Array[0][0]);
		}

		constexpr ColumnType GetColumn(uint32 idx) const
		{
			ASSERT(idx < Row);
			ColumnType out;
			for (uint32 i = 0; i < Column; ++i)
				out[i] = m_Array[i][idx];
			return out;
		}

		constexpr uint32 SizeOfRow() const
		{
			return Row;
		}

		constexpr uint32 SizeOfColumn() const
		{
			return Column;
		}

		constexpr iterator begin()
		{
			return iterator(m_Array, 0);
		}

		constexpr iterator end()
		{
			return iterator(m_Array, Column);
		}

		constexpr const_iterator cbegin() const
		{
			return const_iterator(m_Array, 0);
		}

		constexpr const_iterator cend() const
		{
			return const_iterator(m_Array, Column);
		}
		
		constexpr Matrix& Fill(T value)
		{
			for (uint32 i = 0; i < Column; ++i)
				m_Array[i].Fill(value);
			return *this;
		}

		constexpr Matrix& Apply(T(*func)(T))
		{
			for (uint32 i = 0; i < Column; ++i)
				m_Array[i].Apply(func);
			return *this;
		}

// -------------Operators-------------------------------------
	public:
		constexpr const RowType& operator[](uint32 idx) const
		{
			ASSERT(idx < Column);
			return m_Array[idx];
		}

		constexpr RowType& operator[](uint32 idx)
		{
			ASSERT(idx < Column);
			return m_Array[idx];
		}

		constexpr Matrix& operator+=(const Matrix& other)
		{
			for (uint32 i = 0; i < Column; ++i)
				m_Array[i] += other[i];
			return *this;
		}

		constexpr Matrix& operator-=(const Matrix& other)
		{
			for (uint32 i = 0; i < Column; ++i)
				m_Array[i] -= other[i];
			return *this;
		}

		constexpr Matrix& operator+=(float scalar)
		{
			for (uint32 i = 0; i < Column; ++i)
				m_Array[i] += scalar;
			return *this;
		}

		constexpr Matrix& operator-=(float scalar)
		{
			for (uint32 i = 0; i < Column; ++i)
				m_Array[i] -= scalar;
			return *this;
		}

		constexpr Matrix& operator*=(float scalar)
		{
			for (uint32 i = 0; i < Column; ++i)
				m_Array[i] *= scalar;
			return *this;
		}

		constexpr Matrix& operator/=(float scalar)
		{
			for (uint32 i = 0; i < Column; ++i)
				m_Array[i] /= scalar;
			return *this;
		}

		constexpr Matrix operator+(const Matrix& other) const
		{
			return Matrix(*this) += other;
		}

		constexpr Matrix operator-(const Matrix& other) const
		{
			return Matrix(*this) -= other;
		}

		template <uint32 OtherColumn, uint32 OtherRow>
		constexpr Matrix<T, Column, OtherRow> operator*(const Matrix<T, OtherColumn, OtherRow>& other) const
		{
			static_assert(Row == OtherColumn, "Invalid Matrix multiplication");

			Matrix<T, Column, OtherRow> out(static_cast<T>(0));
			for (uint32 i = 0; i < Column; i++)
			{
				for (uint32 j = 0; j < OtherRow; j++)
				{
					for (uint32 k = 0; k < Row; ++k)
						out[i][j] += m_Array[i][k] * other[k][j];
				}
			}
			return out;
		}

		constexpr Matrix operator+(float scalar) const
		{
			return Matrix(*this) += scalar;
		}

		constexpr Matrix operator-(float scalar) const
		{
			return Matrix(*this) -= scalar;
		}

		constexpr Matrix operator*(float scalar) const
		{
			return Matrix(*this) *= scalar;
		}

		constexpr Matrix operator/(float scalar) const
		{
			return Matrix(*this) /= scalar;
		}

// -------------Static Methods-------------------------------------
	public:
		static constexpr Matrix Identity()
		{
			Matrix out(static_cast<T>(0));

			constexpr uint32 min = Math::Min(Column, Row);
			for (uint32 i = 0; i < min; ++i)
				out[i][i] = static_cast<T>(1);

			return out;
		}

	private:
		RowType m_Array[Column];
	};



// -------------Relative Functions-------------------------------------

	template <typename T, uint32_t Column, uint32_t Row>
	constexpr Matrix<T, Column, Row> operator+(float scalar, const Matrix<T, Column, Row>& mat)
	{
		return mat + scalar;
	}

	template <typename T, uint32_t Column, uint32_t Row>
	constexpr Matrix<T, Column, Row> operator*(float scalar, const Matrix<T, Column, Row>& mat)
	{
		return mat * scalar;
	}

	template <typename T, uint32_t Shared, uint32_t Row>
	constexpr Vector<T, Row> operator*(
		const Vector<T, Shared>& vec, const Matrix<T, Shared, Row>& mat)
	{
		Vector<T, Row> out(static_cast<T>(0));
		for (uint32 i = 0; i < Shared; i++)
		{
			for (uint32 j = 0; j < Row; ++j)
				out[j] += vec[i] * mat[i][j];
		}
		return out;
	}

	template <typename T, uint32_t Column, uint32_t Row>
	constexpr Matrix<T, Row, Column> Transpose(const Matrix<T, Column, Row>& mat)
	{
		Matrix<T, Row, Column> out;
		for (uint32 i = 0; i < Column; ++i)
		{
			for (uint32 j = 0; j < Row; ++j)
				out[j][i] = mat[i][j];
		}
		return out;
	}
}
