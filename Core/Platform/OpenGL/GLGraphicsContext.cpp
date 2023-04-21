#include "GLGraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Core
{
	GLGraphicsContext::GLGraphicsContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		ASSERT(windowHandle);

#ifdef APP_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#endif

		glfwMakeContextCurrent(windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(status);
	}

	void GLGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void GLGraphicsContext::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}
}
