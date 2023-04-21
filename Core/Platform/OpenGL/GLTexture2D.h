#pragma once

#include "Core/Core.h"
#include "Core/Texture.h"


typedef unsigned int GLenum;

namespace Core
{
	class GLTexture2D: public Texture2D
	{
	public:
		GLTexture2D(const FilePath& path, bool sRGB, const TextureSamplerDescription& samplerDesc);
		GLTexture2D(const void* Data, uint32 Width, uint32 Height, bool sRGB, const TextureSamplerDescription& samplerDesc);
		GLTexture2D(TextureFormat Format, uint32 Width, uint32 Height, const TextureSamplerDescription& samplerDesc);

		virtual ~GLTexture2D();

		virtual inline uint32 GetWidth() const override { return m_Width; }
		virtual inline uint32 GetHeight() const override { return m_Height; }

		virtual void SetData(const void* data, uint32 size) override;

		virtual void Bind(uint32 slot = 0) const override;
		virtual void BindAsImage(uint32 slot = 0, uint32 level = 0) override;

		virtual bool IsLoaded() const override { return m_IsLoaded; };

		virtual const FilePath& GetFilePath() const override { return m_FilePath; };

	private:
		void CreateSampler(const TextureSamplerDescription& samplerDesc);

	private:
		FilePath m_FilePath;
		uint32 m_Width = 0, m_Height = 0;
		bool m_IsLoaded = false;
		GLenum m_InternalFormat = 0, m_DataFormat = 0;
		uint32 m_GLRendererID;
	};
}
