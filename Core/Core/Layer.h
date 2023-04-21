#pragma once

#include "Core/Core.h"
#include "Core/Time.h"

#include "Input/Event.h"


namespace Core
{
	class Layer
	{
	public:
		Layer(const String& name = "Layer")
			: m_Name(name) {}

		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Time frameTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const String& GetName() const { return m_Name; }

	protected:
		String m_Name;
	};
}
