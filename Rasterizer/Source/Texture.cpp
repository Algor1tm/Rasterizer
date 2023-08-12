#include "Texture.h"

#include <stb_image/stb_image.h>


namespace Raster
{
	Ref<Texture> Texture::Create(const FilePath& filepath)
	{
		Ref<Texture> texture = Core::CreateRef<Texture>();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		String path = filepath.string();
		void* data;

		data = stbi_load(path.data(), &width, &height, &channels, 0);
		
		if (data)
		{
			texture->m_Width = width;
			texture->m_Height = height;

			if (channels < 3)
			{
				ASSERT(false);
				stbi_image_free(data);
				return texture;
			}


			if (channels == 3)
			{
				using Vector3u8 = Math::Vector<uint8, 3>;

				Vector3u8* first = (Vector3u8*)data;
				Vector3u8* last = (Vector3u8*)data + width * height;

				texture->m_Pixels.reserve(width * height);

				while (first != last)
				{
					Vector3u8 rgb = (*first);
					Pixel pixel = {rgb.x, rgb.y, rgb.z, 255};

					texture->m_Pixels.push_back(pixel);

					first++;
				}
			}
			else if (channels == 4)
			{
				Pixel* first = (Pixel*)data;
				Pixel* last = (Pixel*)data + width * height;

				texture->m_Pixels = std::vector<Pixel>(first, last);
			}

			stbi_image_free(data);
		}

		return texture;
	}

	Vector4 Texture::Sample(Vector2 uv) const
	{
		float x = uv.x * m_Width;
		float y = uv.y * m_Height;

		SampleWrap(m_Sampler.Wrap, x, y);

		Vector4 color = SampleFilter(m_Sampler.Filter, x, y);

		return color;
	}

	void Texture::SampleWrap(Core::TextureWrap wrap, float& x, float& y) const
	{
		switch (wrap)
		{
		case Core::TextureWrap::CLAMP_TO_BORDER:
		{
			x = Math::Clamp((uint32)x, 0, m_Width - 1);
			y = Math::Clamp((uint32)y, 0, m_Height - 1);
			break;
		}
		case Core::TextureWrap::REPEAT:
		{
			x = (int32)x % m_Width;
			y = (int32)y % m_Height;
			break;
		}
		}
	}

	Vector4 Texture::SampleFilter(Core::TextureFilter filter, float x, float y) const
	{
		Vector4 result = Vector4(0, 0, 0, 1);

		switch (filter)
		{
		case Core::TextureFilter::LINEAR:
		{
			float leftX = Math::Clamp(x, 0, m_Width - 1);
			float upY = Math::Clamp(y, 0, m_Height - 1);
			float rightX = Math::Clamp(x + 1, 0, m_Width - 1);
			float downY = Math::Clamp(y + 1, 0, m_Height - 1);

			float leftUpIndex = leftX + m_Width * upY;
			float rightUpIndex = rightX + m_Width * upY;
			float leftDownIndex = leftX + m_Width * downY;
			float rightDownIndex = rightX + m_Width * downY;

			Vector4 leftUp = m_Pixels[leftUpIndex];
			Vector4 rightUp = m_Pixels[rightUpIndex];
			Vector4 leftDown = m_Pixels[leftDownIndex];
			Vector4 rightDown = m_Pixels[rightDownIndex];

			Vector4 up = Math::Lerp(leftUp, rightUp, x / m_Width);
			Vector4 down = Math::Lerp(leftDown, rightDown, x / m_Width);

			result = Math::Lerp(up, down, y / m_Height);
			break;
		}
		case Core::TextureFilter::NEAREST:
		{
			uint64 index = x + y * m_Width;
			result = m_Pixels[index];
			break;
		}
		}

		return result;
	}
}
