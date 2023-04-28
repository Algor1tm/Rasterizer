#pragma once

#include <Math/Vector.h>


namespace Raster
{
	struct VertexShaderInput
	{
		Vector3 Position;
		Vector4 Color;
	};

	struct Interpolators
	{
		Vector4 Color;
	};

	using VertexShader = Interpolators(*)(const VertexShaderInput&, Vector4&);
	using FragmentShader = Vector4(*)(const Interpolators&);
}
