#include <Core/EntryPoint.h>

#include "LayerExample.h"


class ApplicationExample : public Core::Application
{
public:
	ApplicationExample(const Core::ApplicationDescription& appdesc)
		: Application(appdesc)
	{
		// Add layer to application
		PushLayer(Core::CreateRef<LayerExample>());
	}

	~ApplicationExample()
	{

	}
};


namespace Core
{
	Application* CreateApplication()
	{
		ApplicationDescription appdesc;

		appdesc.WindowDesc.Width = 1600;
		appdesc.WindowDesc.Height = 900;
		appdesc.WindowDesc.Title = "Application Example";
		appdesc.WindowDesc.VSync = true;
		appdesc.WindowDesc.Mode = WindowMode::Maximized;
		appdesc.WindowDesc.Icon = "";

		appdesc.EnableConsole = true;
		appdesc.EnableImGui = true;
		appdesc.WorkingDirectory = "";

		return new ApplicationExample(appdesc);
	}
}
