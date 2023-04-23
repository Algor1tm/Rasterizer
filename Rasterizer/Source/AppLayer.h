#pragma once

#include <Core/Layer.h>
#include <Input/ApplicationEvent.h>

#include "RenderTarget.h"
#include "Rasterizer.h"
#include "SwapChain.h"
#include "Buffers.h"


namespace Raster
{
	class AppLayer : public Core::Layer
	{
	public:
		AppLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnImGuiRender() override;
		void OnUpdate(Core::Time frameTime) override;
		void OnEvent(Core::Event& event) override;

	private:
		bool OnWindowResize(Core::WindowResizeEvent& event);

	private:
		Core::Time m_FrameTime;
		Ref<RenderTarget> m_RenderTarget;
		Ref<Rasterizer> m_Rasterizer;
		Ref<SwapChain> m_SwapChain;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		Ref<VertexBuffer> m_LineBuffer;
		Ref<IndexBuffer> m_LineIndexBuffer;

		Vector2 m_ViewportSize = { 1200, 720 };
	};
}
