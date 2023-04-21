#pragma once

#include "Core/Core.h"
#include "Core/Time.h"
#include "Core/LayerStack.h"
#include "Core/Window.h"

#include "Input/ApplicationEvent.h"


namespace Core
{
	class ImGuiLayer;


	struct ApplicationDescription
	{
		WindowDescription WindowDesc;
		bool EnableImGui = true;
		bool EnableConsole = true;
		FilePath WorkingDirectory = FilePath();
	};


	class Application
	{
	public:
		Application(const ApplicationDescription& appdesc);
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Ref<Layer> layer);
		void PushOverlay(Ref<Layer> layer);

		inline Ref<ImGuiLayer> GetImGuiLayer() { return m_ImGuiLayer; }
		inline Window& GetWindow() { return *m_Window; }

		void Close();

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

		Scope<Window> m_Window;
		Ref<ImGuiLayer> m_ImGuiLayer;
		bool m_Running;
		bool m_Minimized;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
	};

	// Defined by user
	Application* CreateApplication();
}
