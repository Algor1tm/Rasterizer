#pragma once

#include "Pixel.h"


namespace Raster
{
	class RenderTarget
	{
	public:
		static Ref<RenderTarget> Create(uint32 width, uint32 height);

		void Resize(uint32 width, uint32 height);

		Pixel& Get(uint32 x, uint32 y) { return m_Pixels[x + y * m_Width]; }
		const Pixel& Get(uint32 x, uint32 y) const { return m_Pixels[x + y * m_Width]; }

		const Pixel* Data() const { return m_Pixels.data(); }

		std::vector<Pixel>& ReadAllPixels() { return m_Pixels; }

		uint32 GetWidth() const { return m_Width; };
		uint32 GetHeight() const { return m_Height; };

	private:
		std::vector<Pixel> m_Pixels;
		uint32 m_Width;
		uint32 m_Height;
	};
}
