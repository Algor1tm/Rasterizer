#pragma once

#include "Core/Core.h"

#include "Input/Event.h"

#include "Math/Vector.h"


namespace Core
{
	class GraphicsContext;


	enum class WindowMode
	{
		Default = 0,
		Maximized = 1,
		Minimized = 2,
		Fullscreen = 3
	};

	struct WindowDescription
	{
		using EventCallbackFn = std::function<void(Event&)>;

		uint32 Width = 1280; 
		uint32 Height = 720;
		String Title = "Athena App";
		bool VSync = true;
		WindowMode Mode = WindowMode::Default;
		FilePath Icon;

		EventCallbackFn EventCallback = [](Event&) {};
	};

	class Window
	{
	public:
		static Scope<Window> Create(const WindowDescription& desc);

		~Window();
		
		void OnUpdate();

		inline uint32 GetWidth() const { return m_Data.Width; }
		inline uint32 GetHeight() const { return m_Data.Height; }

		inline void SetEventCallback(const WindowDescription::EventCallbackFn& callback)
		{
			m_Data.EventCallback = callback;
		}

		void SetVSync(bool enabled);
		bool IsVSync() const { return m_Data.VSync; }

		inline void* GetNativeWindow() { return m_WindowHandle; }

		void SetWindowMode(WindowMode mode);
		inline WindowMode GetWindowMode() const { return m_Data.Mode; }

		void HideCursor(bool hide);
		void SetCursorPosition(Vector2 position);

		const Ref<GraphicsContext>& GetGraphicsContext() const { return m_Context; }

	public:
		struct WindowData
		{
			uint32 Width = 1280;
			uint32 Height = 720;
			bool VSync = true;
			String Title = "Athena App";
			WindowMode Mode = WindowMode::Default;

			WindowDescription::EventCallbackFn EventCallback = [](Event&) {};
		};

	private:
		void* m_WindowHandle = nullptr;
		WindowData m_Data;
		Ref<GraphicsContext> m_Context;

		static inline uint8 m_WindowCount = 0;
	};
}
