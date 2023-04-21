#pragma once

#include "Core/Core.h"
#include "Core/Color.h"

#include "Math/Vector.h"


namespace Core
{
	enum class TextureFormat
	{
		NONE = 0,

		// Color
		RGB8,
		RGBA8,

		RG16F,
		R11F_G11F_B10F,
		RGB16F,
		RGB32F,
		RGBA16F,

		RED_INTEGER,

		//Depth/Stencil
		DEPTH24STENCIL8,
		DEPTH32F
	};

	enum class TextureFilter
	{
		LINEAR = 1,
		NEAREST = 2,
		LINEAR_MIPMAP_LINEAR = 3
	};

	enum class TextureWrap
	{
		REPEAT = 1,
		CLAMP_TO_EDGE = 2,
		CLAMP_TO_BORDER = 3,
		MIRRORED_REPEAT = 4,
		MIRRORED_CLAMP_TO_EDGE = 5
	};

	struct TextureSamplerDescription
	{
		TextureFilter MinFilter = TextureFilter::LINEAR;
		TextureFilter MagFilter = TextureFilter::LINEAR;
		TextureWrap Wrap = TextureWrap::REPEAT;

		LinearColor BorderColor = LinearColor::Black;
	};


	class Texture
	{
	public:
		virtual ~Texture() = default;

		inline void* GetRendererID() const { return (void*)m_RendererID; };

		virtual void Bind(uint32 slot = 0) const = 0;
		virtual void BindAsImage(uint32 slot = 0, uint32 level = 0) = 0;

		virtual bool IsLoaded() const = 0;

	protected:
		uint64 m_RendererID = 0;
	};


	class Texture2D: public Texture
	{
	public:
		static Ref<Texture2D> Create(const FilePath& path, 
			bool sRGB = false, const TextureSamplerDescription& samplerDesc = TextureSamplerDescription());

		static Ref<Texture2D> Create(const void* Data, uint32 Width, uint32 Height, 
			bool sRGB = false, const TextureSamplerDescription& samplerDesc = TextureSamplerDescription());

		static Ref<Texture2D> Create(TextureFormat Format, uint32 Width, uint32 Height, 
			const TextureSamplerDescription& samplerDesc = TextureSamplerDescription());

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual void SetData(const void* data, uint32 size) = 0;
		virtual const FilePath& GetFilePath() const = 0;

		bool operator==(const Texture2D& other) const { return GetRendererID() == other.GetRendererID(); }
	};
}
