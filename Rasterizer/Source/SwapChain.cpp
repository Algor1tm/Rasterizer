#include "SwapChain.h"


namespace Raster
{
	Ref<SwapChain> SwapChain::Create(uint32 width, uint32 height)
	{
		Ref<SwapChain> swapChain = Core::CreateRef<SwapChain>();
		swapChain->Resize(width, height);

		return swapChain;
	}

	void SwapChain::Resize(uint32 width, uint32 height)
	{
		m_NativeTexture = Core::Texture2D::Create(Core::TextureFormat::RGBA8, width, height);
	}

	void SwapChain::SwapBuffers(Ref<RenderTarget> target)
	{
		uint32 size = target->GetWidth() * target->GetHeight() * sizeof(Pixel);
		m_NativeTexture->SetData(target->Data(), size);
	}

	void* SwapChain::GetNativeTextureHandle() const
	{
		return m_NativeTexture->GetRendererID();
	}
}
