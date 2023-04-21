#pragma once

#include "Core/Core.h"

#include "ImGui/ImGuiLayer.h"


namespace Core
{
	class GLImGuiLayerImpl: public ImGuiLayerImpl
	{
	public:
		virtual void Init(void* windowHandle) override;
		virtual void Shutdown() override;

		virtual void NewFrame() override;
		virtual void RenderDrawData() override;

		virtual void UpdateViewports() override;
	};
}
