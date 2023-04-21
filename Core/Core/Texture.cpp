#include "Texture.h"

#include "Platform/OpenGL/GLTexture2D.h"


namespace Core
{
	Ref<Texture2D> Texture2D::Create(const FilePath& path, bool sRGB, const TextureSamplerDescription& samplerDesc)
	{
		return CreateRef<GLTexture2D>(path, sRGB, samplerDesc);
	}

	Ref<Texture2D> Texture2D::Create(const void* data, uint32 width, uint32 height, bool sRGB, const TextureSamplerDescription& samplerDesc)
	{
		return CreateRef<GLTexture2D>(data, width, height, sRGB, samplerDesc);
	}

	Ref<Texture2D> Texture2D::Create(TextureFormat format, uint32 width, uint32 height, const TextureSamplerDescription& samplerDesc)
	{
		return CreateRef<GLTexture2D>(format, width, height, samplerDesc);
	}
}
