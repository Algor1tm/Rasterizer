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
					Pixel pixel = {rgb.x, rgb.y, rgb.z, 1};

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
		uint32 x = uv.x * m_Width;
		uint32 y = uv.y * m_Height;

		uint64 index = x + y * m_Width;

		// NEAREST
		index = Math::Clamp(index, 0, m_Width * m_Height - 1);

		return m_Pixels[index];
	}

	Vector4 Texture::Get(Vector2 uv) const
	{
		uint32 x = uv.x * m_Width;
		uint32 y = uv.y * m_Height;

		uint64 index = x + y * m_Width;
		index = index == m_Pixels.size() ? m_Pixels.size() - 1 : index; // (uv == (1, 1) => last pixel)

		return m_Pixels[index];
	}
}
