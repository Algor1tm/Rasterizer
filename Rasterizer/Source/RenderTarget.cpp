#include "RenderTarget.h"


namespace Raster
{
	Ref<RenderTarget> RenderTarget::Create(uint32 width, uint32 height)
	{
		Ref<RenderTarget> target = Core::CreateRef<RenderTarget>();
		target->Resize(width, height);

		return target;
	}

	void RenderTarget::Resize(uint32 width, uint32 height)
	{
		m_Width = width;
		m_Height = height;

		m_Pixels.resize(width * height);
	}
}
