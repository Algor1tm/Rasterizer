#include "RenderTarget.h"


namespace Raster
{
	Ref<RenderTarget> RenderTarget::Create(uint32 width, uint32 height)
	{
		Ref<RenderTarget> target = Core::CreateRef<RenderTarget>();
		target->Resize(width, height);

		return target;
	}

	void RenderTarget::Clear(Core::LinearColor color)
	{
		Pixel newPixel;
		newPixel.Red = uint8(255 * color.r);
		newPixel.Green = uint8(255 * color.g);
		newPixel.Blue = uint8(255 * color.b);
		newPixel.Alpha = uint8(255 * color.a);

		for (auto& pixel : m_Pixels)
			pixel = newPixel;
	}

	void RenderTarget::Resize(uint32 width, uint32 height)
	{
		m_Width = width;
		m_Height = height;

		m_Pixels.resize(width * height);
	}
}
