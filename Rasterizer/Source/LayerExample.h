#pragma once

#include <Core/Layer.h>
#include <Input/ApplicationEvent.h>


class LayerExample : public Core::Layer
{
public:
	LayerExample();

	void OnAttach() override;
	void OnDetach() override;

	void OnImGuiRender() override;
	void OnUpdate(Core::Time frameTime) override;
	void OnEvent(Core::Event& event) override;

private:
	bool OnWindowResize(Core::WindowResizeEvent& event);
};
