#include "Application.h"

#include "Core/Time.h"

#include "ImGui/ImGuiLayer.h"

#include "Input/Input.h"


#ifdef APP_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <stdio.h>
	#include <fcntl.h>
	#include <io.h>
	#include <iostream>
	#include <fstream>

	// maximum mumber of lines the output console should have
	static const WORD MAX_CONSOLE_LINES = 1000;

	static void CreateConsole()
	{
		int hConHandle;
		__int64 lStdHandle;
		CONSOLE_SCREEN_BUFFER_INFO coninfo;
		FILE* fp;

		// allocate a console for this app
		AllocConsole();

		// set the screen buffer to be big enough to let us scroll text
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
		coninfo.dwSize.Y = MAX_CONSOLE_LINES;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

		// redirect unbuffered STDOUT to the console
		lStdHandle = reinterpret_cast<__int64>(GetStdHandle(STD_OUTPUT_HANDLE));
		hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
		fp = _fdopen(hConHandle, "w");
		*stdout = *fp;
		setvbuf(stdout, NULL, _IONBF, 0);

		std::ios::sync_with_stdio();
	}

#else

	static void CreateConsole()
	{

	}

#endif

namespace Core
{

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationDescription& appdesc)
		: m_Running(true), m_Minimized(false)
	{
		ASSERT(s_Instance == nullptr);
		s_Instance = this;

		if (appdesc.WorkingDirectory != FilePath())
			std::filesystem::current_path(appdesc.WorkingDirectory);

		if (appdesc.EnableConsole)
			CreateConsole();
			
		m_Window = Window::Create(appdesc.WindowDesc);

		m_Window->SetEventCallback(APP_BIND_EVENT_FN(Application::OnEvent));

		if (appdesc.EnableImGui)
		{
			m_ImGuiLayer = ImGuiLayer::Create();
			PushOverlay(m_ImGuiLayer);
		}
		else
		{
			m_ImGuiLayer = nullptr;
		}
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(APP_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(APP_BIND_EVENT_FN(Application::OnWindowResized));

		for (LayerStack::iterator it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	void Application::Run()
	{
		Timer timer;
		Time frameTime = 0;

		while (m_Running)
		{
			Time start = timer.ElapsedTime();

			if (m_Minimized == false)
			{
				{
					Time start = timer.ElapsedTime();

					for (Ref<Layer> layer : m_LayerStack)
					{
						layer->OnUpdate(frameTime);
					}
				}

				if (m_ImGuiLayer != nullptr)
				{
					m_ImGuiLayer->Begin();
					{
						for (Ref<Layer> layer : m_LayerStack)
							layer->OnImGuiRender();
					}
					m_ImGuiLayer->End();
				}
			}

			{
				m_Window->OnUpdate();
			}

			frameTime = timer.ElapsedTime() - start;
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::PushLayer(Ref<Layer> layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Ref<Layer> layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& event)
	{
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		return false;
	}
}
