#pragma once

#include "Core/Core.h"


namespace Core
{
	class GraphicsContext
	{
	public:
		static Ref<GraphicsContext> Create(void* windowHandle);
		virtual ~GraphicsContext() = default;

		virtual void SwapBuffers() = 0;
		virtual void SetVSync(bool enabled) = 0;
	};
}
