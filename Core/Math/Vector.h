#pragma once


#include "TypesImpl/Vector.h"
#include "TypesImpl/Vector2.h"
#include "TypesImpl/Vector3.h"
#include "TypesImpl/Vector4.h"

#include "VectorCommon.h"
#include "VectorTrigonometric.h"
#include "VectorExponential.h"



using Vector2  = Math::Vector<float, 2>;
using Vector2f = Math::Vector<float, 2>;
using Vector2u = Math::Vector<Core::uint32, 2>;
using Vector2i = Math::Vector<int32_t, 2>;
using Vector2d = Math::Vector<double, 2>;
				 
using Vector3  = Math::Vector<float, 3>;
using Vector3f = Math::Vector<float, 3>;
using Vector3u = Math::Vector<Core::uint32, 3>;
using Vector3i = Math::Vector<int32_t, 3>;
using Vector3d = Math::Vector<double, 3>;
				 
using Vector4  = Math::Vector<float, 4>;
using Vector4f = Math::Vector<float, 4>;
using Vector4u = Math::Vector<Core::uint32, 4>;
using Vector4i = Math::Vector<int32_t, 4>;
using Vector4d = Math::Vector<double, 4>;

namespace Core
{
	template <typename T, uint32 Size>
	using Vector = Math::Vector<T, Size>;
}
