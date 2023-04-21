#pragma once

#include "Core/Core.h"
#include "Core/Layer.h"

#include "Input/Event.h"


namespace Core
{
	class ImGuiLayerImpl
	{
	public:
		virtual void Init(void* windowHandle) = 0;
		virtual void Shutdown() = 0;

		virtual void NewFrame() = 0;
		virtual void RenderDrawData() = 0;

		virtual void UpdateViewports() = 0;
	};


	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		static Ref<ImGuiLayer> Create();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();
		
		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.f;

		Scope<ImGuiLayerImpl> m_ImGuiImpl;
	};
}
