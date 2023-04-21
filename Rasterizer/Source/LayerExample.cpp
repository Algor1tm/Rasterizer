#include "LayerExample.h"

#include <ImGui/imgui.h>


LayerExample::LayerExample()
	: Layer("LayerExample")
{

}

void LayerExample::OnAttach()
{

}

void LayerExample::OnDetach()
{

}

void LayerExample::OnUpdate(Core::Time frameTime)
{

}

void LayerExample::OnImGuiRender()
{
	static bool dockSpaceOpen = true;
	static constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	}

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
	ImGui::Begin("Viewport");

	const char* text = "SOMETHING COOL";

	ImGui::SetWindowFontScale(5.f);

	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	ImVec2 textSize = ImGui::CalcTextSize(text);

	ImVec2 position = { viewportSize.x / 2.f - textSize.x / 2.f, viewportSize.y / 2.f  - textSize.y / 2.f };

	ImGui::SetCursorPos(position);
	ImGui::Text(text);

	ImGui::SetWindowFontScale(1.f);

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Begin("Settings");

	static bool check = true;
	ImGui::Checkbox("Checkbox", &check);
	ImGui::Text("This variable is %s", check ? "true" : "false");

	ImGui::End();
}

bool LayerExample::OnWindowResize(Core::WindowResizeEvent& event)
{	
	// Handle window resize event
	return false;
}

void LayerExample::OnEvent(Core::Event& event)
{
	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::WindowResizeEvent>(APP_BIND_EVENT_FN(OnWindowResize));
}
