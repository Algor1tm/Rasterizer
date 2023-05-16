#pragma once

#include "Pixel.h"

#include <Core/Texture.h>


namespace Raster
{
	struct Sampler
	{
		Core::TextureFilter MinFilter = Core::TextureFilter::LINEAR;
		Core::TextureFilter Magilter = Core::TextureFilter::LINEAR;

		Core::TextureWrap Wrap = Core::TextureWrap::REPEAT;
	};


	class Texture
	{
	public:
		static Ref<Texture> Create(const FilePath& path);

		uint32 GetWidth() const { return m_Width; }
		uint32 GetHeight() const { return m_Width; }

		Vector4 Sample(Vector2 uv) const;

		Sampler& GetSampler() { return m_Sampler; }
		void SetSampler(Sampler sampler) { m_Sampler = sampler; }

	private:
		Sampler m_Sampler;
		std::vector<Pixel> m_Pixels;
		uint32 m_Width;
		uint32 m_Height;
	};
}
