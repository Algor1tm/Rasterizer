#pragma once

#include "Core/Core.h"

#include "Core/GraphicsContext.h"


struct GLFWwindow;

namespace Core
{
	class GLGraphicsContext : public GraphicsContext
	{
	public:
		GLGraphicsContext(GLFWwindow* windowHandle);

		virtual void SwapBuffers() override;
		virtual void SetVSync(bool enabled) override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}
