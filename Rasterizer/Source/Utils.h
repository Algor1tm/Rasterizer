#pragma once

#include <ImGui/imgui.h>


namespace Raster
{
	inline std::string_view TextureWrapToString(Core::TextureWrap wrap)
	{
		switch (wrap)
		{
		case Core::TextureWrap::CLAMP_TO_BORDER: return "CLAMP_TO_BORDER";
		case Core::TextureWrap::REPEAT: return "REPEAT";
		}

		return "";
	}

	inline std::string_view TextureFilterToString(Core::TextureFilter filter)
	{
		switch (filter)
		{
		case Core::TextureFilter::LINEAR: return "LINEAR";
		case Core::TextureFilter::NEAREST: return "NEAREST";
		}

		return "";
	}

	inline void Spacing(uint32 size)
	{
		for (uint32 i = 0; i < size; ++i)
			ImGui::Spacing();
	}

	inline void DrawVec3Controller(std::string_view label, Vector3& values, float defaultValues, float height)
	{
		ImGui::PushID(label.data());

		float full_width = ImGui::GetContentRegionAvail().x - 15.f;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

		float buttonWidth = ImGui::GetFrameHeight();
		ImVec2 buttonSize = { buttonWidth, buttonWidth };

		float dragWidth = (full_width - 3 * buttonWidth) / 5.f;

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.f });

		if (ImGui::Button("X", buttonSize))
			values.x = defaultValues;

		ImGui::SameLine();
		ImGui::PushItemWidth(dragWidth);
		ImGui::DragFloat("##X", &values.x, 0.07f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f, 0.7f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.8f, 0.3f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2f, 0.7f, 0.2f, 1.f });

		if (ImGui::Button("Y", buttonSize))
			values.y = defaultValues;

		ImGui::SameLine();
		ImGui::PushItemWidth(dragWidth);
		ImGui::DragFloat("##Y", &values.y, 0.07f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.25f, 0.8f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2f, 0.35f, 0.9f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.25f, 0.8f, 1.f });

		if (ImGui::Button("Z", buttonSize))
			values.z = defaultValues;

		ImGui::SameLine();
		ImGui::PushItemWidth(dragWidth);
		ImGui::DragFloat("##Z", &values.z, 0.7f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();

		ImGui::PopID();
	}
}
