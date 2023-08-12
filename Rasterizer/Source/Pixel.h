#pragma once

#include <Math/Vector.h>


namespace Raster
{
	struct Pixel
	{
		Pixel() = default;
		Pixel(uint8 r, uint8 g, uint8 b, uint8 a = 255)
			: Red(r), Green(g), Blue(b), Alpha(a)
		{}

		Pixel(Vector4 norm)
			: Red(norm.x * 255), Green(norm.y * 255), Blue(norm.z * 255), Alpha(norm.w * 255)
		{}

		operator Vector4() const
		{
			return { (float)Red / 255.f, (float)Green / 255.f, (float)Blue / 255.f, (float)Alpha / 255.f };
		}

		uint8 Red = 0;
		uint8 Green = 0;
		uint8 Blue = 0;
		uint8 Alpha = 255;
	};
}
