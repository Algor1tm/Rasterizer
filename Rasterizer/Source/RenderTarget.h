#pragma once

#include <Core/Color.h>


namespace Raster
{
	struct Pixel
	{
		Pixel() = default;
		Pixel(uint8 r, uint8 g, uint8 b, uint8 a = 255)
			: Red(r), Green(g), Blue(b), Alpha(a)
		{

		}

		uint8 Red = 0;
		uint8 Green = 0;
		uint8 Blue = 0;
		uint8 Alpha = 255;
	};


	class RenderTarget
	{
	public:
		static Ref<RenderTarget> Create(uint32 width, uint32 height);

		void Clear(Core::LinearColor color);
		void Resize(uint32 width, uint32 height);

		Pixel& Get(uint32 x, uint32 y) { return m_Pixels[x + y * m_Width]; }
		const Pixel& Get(uint32 x, uint32 y) const { return m_Pixels[x + y * m_Width]; }

		const Pixel* Data() const { return m_Pixels.data(); }

		uint32 GetWidth() const { return m_Width; };
		uint32 GetHeight() const { return m_Height; };

	private:
		std::vector<Pixel> m_Pixels;
		uint32 m_Width;
		uint32 m_Height;
	};
}
