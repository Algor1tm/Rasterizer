#pragma once

#include <Core/Texture.h>

#include "RenderTarget.h"


namespace Raster
{
	class SwapChain
	{
	public:
		static Core::Ref<SwapChain> Create(uint32 width, uint32 height);

		void Resize(uint32 width, uint32 height);

		void SwapBuffers(Ref<RenderTarget> target);
		void* GetNativeTextureHandle() const;

	private:
		Ref<Core::Texture2D> m_NativeTexture;
	};
}
