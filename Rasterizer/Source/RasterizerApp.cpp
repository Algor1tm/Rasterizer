#include <Core/EntryPoint.h>

#include "AppLayer.h"


namespace Raster
{
	class RasterizerApplication : public Core::Application
	{
	public:
		RasterizerApplication(const Core::ApplicationDescription& appdesc)
			: Application(appdesc)
		{
			PushLayer(Core::CreateRef<AppLayer>());
		}

		~RasterizerApplication()
		{

		}
	};
}

namespace Core
{
	Application* CreateApplication()
	{
		ApplicationDescription appdesc;
		appdesc.WindowDesc.Width = 1600;
		appdesc.WindowDesc.Height = 900;
		appdesc.WindowDesc.Title = "Software Rasterizer";
		appdesc.WindowDesc.VSync = true;
		appdesc.WindowDesc.Mode = WindowMode::Maximized;
		appdesc.WindowDesc.Icon = "";

		appdesc.EnableImGui = true;
		appdesc.EnableConsole = false;
		appdesc.WorkingDirectory = "";

		return new Raster::RasterizerApplication(appdesc);
	}
}
