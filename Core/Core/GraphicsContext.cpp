#include "GraphicsContext.h"

#include "Platform/OpenGL/GLGraphicsContext.h"


namespace Core
{
	Ref<GraphicsContext> GraphicsContext::Create(void *windowHandle)
	{
		return CreateRef<GLGraphicsContext>(reinterpret_cast<GLFWwindow*>(windowHandle));
	}
}
