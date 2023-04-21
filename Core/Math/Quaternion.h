#pragma once

#include "TypesImpl/Quaternion.h"

#include "QuaternionCommon.h"



using Quaternion = Math::Quaternion<float>;
using FQuaternion = Math::Quaternion<float>;
using DQuaternion = Math::Quaternion<double>;

namespace Core
{
	template <typename T>
	using Quaterion = Math::Quaternion<T>;
}
