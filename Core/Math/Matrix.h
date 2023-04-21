#pragma once

#include "TypesImpl/Matrix.h"
#include "TypesImpl/Matrix4.h"

#include "MatrixCommon.h"


using Matrix2 = Math::Matrix<float, 2, 2>;
using Matrix3 = Math::Matrix<float, 3, 3>;
using Matrix4 = Math::Matrix<float, 4, 4>;

using Matrix2f = Math::Matrix<float, 2, 2>;
using Matrix3f = Math::Matrix<float, 3, 3>;
using Matrix4f = Math::Matrix<float, 4, 4>;

using Matrix2i = Math::Matrix<int32_t, 2, 2>;
using Matrix3i = Math::Matrix<int32_t, 3, 3>;
using Matrix4i = Math::Matrix<int32_t, 4, 4>;

using Matrix2d = Math::Matrix<double, 2, 2>;
using Matrix3d = Math::Matrix<double, 3, 3>;
using Matrix4d = Math::Matrix<double, 4, 4>;

using Matrix2x3 = Math::Matrix<float, 2, 3>;
using Matrix3x2 = Math::Matrix<float, 3, 2>;
using Matrix2x4 = Math::Matrix<float, 2, 4>;
using Matrix4x2 = Math::Matrix<float, 4, 2>;
using Matrix3x4 = Math::Matrix<float, 3, 4>;
using Matrix4x3 = Math::Matrix<float, 4, 3>;

using Matrix2x3f = Math::Matrix<float, 2, 3>;
using Matrix3x2f = Math::Matrix<float, 3, 2>;
using Matrix2x4f = Math::Matrix<float, 2, 4>;
using Matrix4x2f = Math::Matrix<float, 4, 2>;
using Matrix3x4f = Math::Matrix<float, 3, 4>;
using Matrix4x3f = Math::Matrix<float, 4, 3>;

using Matrix2x3i = Math::Matrix<int32_t, 2, 3>;
using Matrix3x2i = Math::Matrix<int32_t, 3, 2>;
using Matrix2x4i = Math::Matrix<int32_t, 2, 4>;
using Matrix4x2i = Math::Matrix<int32_t, 4, 2>;
using Matrix3x4i = Math::Matrix<int32_t, 3, 4>;
using Matrix4x3i = Math::Matrix<int32_t, 4, 3>;

using Matrix2x3d = Math::Matrix<double, 2, 3>;
using Matrix3x2d = Math::Matrix<double, 3, 2>;
using Matrix2x4d = Math::Matrix<double, 2, 4>;
using Matrix4x2d = Math::Matrix<double, 4, 2>;
using Matrix3x4d = Math::Matrix<double, 3, 4>;
using Matrix4x3d = Math::Matrix<double, 4, 3>;


namespace Core
{
	template <typename T, uint32 Column, uint32 Row>
	using Matrix = Math::Matrix<T, Column, Row>;
}
