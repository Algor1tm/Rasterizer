#include "AppLayer.h"

#include "Utils.h"

#include <Core/Application.h>
#include <ImGui/ImGuiLayer.h>

#include <ImGui/imgui.h>


namespace Raster
{
	AppLayer::AppLayer()
		: Layer("RasterizerLayer"), m_Camera(16.f / 9.f, true)
	{
		m_SwapChain = SwapChain::Create(1, 1);
		m_RenderTarget = RenderTarget::Create(1, 1);
		
		RasterizerState state;
		m_Rasterizer = Rasterizer::Create(state);

		InitVertexBuffers();

		m_Emoji_32 = Texture::Create("Assets/Textures/AwesomeFace32x32.png");
		m_Emoji_128 = Texture::Create("Assets/Textures/AwesomeFace128x128.png");
		m_Wallpapers = Texture::Create("Assets/Textures/Wallpapers256x256.jpg");

		Core::Application::Get().GetImGuiLayer()->BlockEvents(false);
		Core::Application::Get().GetWindow().SetVSync(false);

		m_RectTransform.Rotation.z = Math::PI<float>() / 4;
		m_RectTransform.Scale *= 2;
	}

	void AppLayer::InitVertexBuffers()
	{
		float vertices[] = { -0.3f, -0.3f, 0,   1, 0, 0, 1,   0.f, 0.f,
							 -0.3f,  0.3f, 0,   0, 1, 0, 1,   0.f, 1.f,
							  0.3f, -0.3f, 0,   0, 0, 1, 1,   1.f, 0.f,
							  0.3f,  0.3f, 0,   1, 0, 0, 1,   1.f, 1.f, };

		uint32 indices[] = { 2, 1, 3, 0, 1, 2,  };

		VertexBufferCreateInfo triangleInfo;
		triangleInfo.Data = (Vertex*)vertices;
		triangleInfo.Size = std::size(vertices) / (sizeof(Vertex) / sizeof(float));
		triangleInfo.Primitives = PrimitiveType::TRIANGLE_LIST;
		triangleInfo.IndexBuffer = IndexBuffer::Create(indices, std::size(indices));

		m_QuadVertexBuffer = VertexBuffer::Create(triangleInfo);

		uint32 rectIndices[] = { 0, 1, 1, 3, 3, 2, 2, 0 };

		VertexBufferCreateInfo rectInfo;
		rectInfo.Data = (Vertex*)vertices;
		rectInfo.Size = std::size(vertices) / (sizeof(Vertex) / sizeof(float));
		rectInfo.Primitives = PrimitiveType::LINE_LIST;
		rectInfo.IndexBuffer = IndexBuffer::Create(rectIndices, std::size(rectIndices));

		m_LineVertexBuffer = VertexBuffer::Create(rectInfo);
	}

	void AppLayer::OnAttach()
	{

	}

	void AppLayer::OnDetach()
	{

	}

	void AppLayer::OnUpdate(Core::Time frameTime)
	{
		m_FrameTime = frameTime;

		if (m_RenderTarget->GetWidth() != m_ViewportSize.x || m_RenderTarget->GetHeight() != m_ViewportSize.y)
		{
			m_SwapChain->Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_RenderTarget->Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_Camera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

#if 0
		float scaleLength = m_QuadTransform.Scale.Length();
		static int sign = 1;
		if (scaleLength >= 2 && sign > 0)
			sign = -1;
		
		if (scaleLength <= 0.2f && sign < 0)
			sign = 1;

		m_QuadTransform.Scale += Vector3(sign * 1 * frameTime.AsSeconds());

		m_QuadTransform.Rotation.z += 1 * frameTime.AsSeconds();
#endif

		m_Camera.OnUpdate(frameTime);
		Render();
	}

	void AppLayer::Render()
	{
		if (m_LockToWindowSize)
		{
			m_RasterizerViewport.X = 0;
			m_RasterizerViewport.Y = 0;
			m_RasterizerViewport.Width = m_ViewportSize.x;
			m_RasterizerViewport.Height = m_ViewportSize.y;

			m_RasterizerScissors.X = 0;
			m_RasterizerScissors.Y = 0;
			m_RasterizerScissors.Width = m_ViewportSize.x;
			m_RasterizerScissors.Height = m_ViewportSize.y;
		}

		// Quads
		{
			RenderPass quadRenderPass;
			quadRenderPass.ClearColor = m_ClearColor;
			quadRenderPass.ClearBit = ClearBit::CLEAR_COLOR_BIT;
			quadRenderPass.OutputRenderTarget = m_RenderTarget;
			BIND_SHADER(quadRenderPass, m_QuadShader);
			quadRenderPass.Viewport = m_RasterizerViewport;
			quadRenderPass.Scissors = m_RasterizerScissors;

			m_Rasterizer->BeginRenderPass(quadRenderPass);

			m_Rasterizer->BindTexture(m_Emoji_128, 0);
			m_Rasterizer->BindTexture(m_Emoji_32, 1);
			m_Rasterizer->BindTexture(m_Wallpapers, 2);

			m_QuadShader.u_MVPMatrix = m_QuadTransform.AsMatrix() * m_Camera.GetViewProjectionMatrix();
			m_Rasterizer->DrawElements(m_QuadVertexBuffer);

			m_Rasterizer->EndRenderPass();
		}

		// Lines
		{
			RenderPass lineRenderPass;
			lineRenderPass.ClearColor = m_ClearColor;
			lineRenderPass.ClearBit = ClearBit::CLEAR_NONE_BIT;
			lineRenderPass.OutputRenderTarget = m_RenderTarget;
			BIND_SHADER(lineRenderPass, m_LineShader);
			lineRenderPass.Viewport = m_RasterizerViewport;
			lineRenderPass.Scissors = m_RasterizerScissors;

			m_Rasterizer->BeginRenderPass(lineRenderPass);

			m_LineShader.u_MVPMatrix = m_RectTransform.AsMatrix() * m_Camera.GetViewProjectionMatrix();
			m_Rasterizer->DrawElements(m_LineVertexBuffer);

			m_Rasterizer->EndRenderPass();
		}


		m_SwapChain->SwapBuffers(m_RenderTarget);
	}

	void AppLayer::OnImGuiRender()
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

		m_IsViewportHovered = ImGui::IsWindowHovered();
		const auto& [viewportX, viewportY] = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportX, viewportY };

		ImGui::Image(m_SwapChain->GetNativeTextureHandle(), { (float)m_RenderTarget->GetWidth(), (float)m_RenderTarget->GetHeight() }, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

		GeneralEditor();
		MeshesEditor();
		RasterizerEditor();
	}

	void AppLayer::OnEvent(Core::Event& event)
	{
		if (m_IsViewportHovered)
		{
			m_Camera.OnEvent(event);
		}
	}

	void AppLayer::GeneralEditor()
	{
		ImGui::Begin("General");

		ImGui::Text("FPS: %.3f", 1.f / m_FrameTime.AsSeconds());

		bool oldVSync = Core::Application::Get().GetWindow().IsVSync();
		bool newVSync = oldVSync;
		ImGui::Checkbox("VSync", &newVSync);

		if (oldVSync != newVSync)
			Core::Application::Get().GetWindow().SetVSync(newVSync);

		ImGui::End();
	}

	void AppLayer::MeshesEditor()
	{
		ImGui::Begin("Meshes Editor");

		if (ImGui::Button("Revert Vertex Buffers"))
		{
			InitVertexBuffers();
		}

		if (BeginTreeNode("QUAD"))
		{
			float height = ImGui::GetFrameHeight();

			ImGui::Text("TRANSFORM");

			DrawVec3Controller("Translation", m_QuadTransform.Translation, 0.0f, height);
			Vector3 degrees = Math::Degrees(m_QuadTransform.Rotation);
			DrawVec3Controller("Rotation", degrees, 0.0f, height);
			m_QuadTransform.Rotation = Math::Radians(degrees);
			DrawVec3Controller("Scale", m_QuadTransform.Scale, 1.0f, height);

			Spacing(2);

			ImGui::Text("VERTICES");

			VertexBufferEditor(m_QuadVertexBuffer);
			EndTreeNode();
		}

		if (BeginTreeNode("RECT"))
		{
			float height = ImGui::GetFrameHeight();

			ImGui::Text("TRANSFORM");

			DrawVec3Controller("Translation", m_RectTransform.Translation, 0.0f, height);
			Vector3 degrees = Math::Degrees(m_RectTransform.Rotation);
			DrawVec3Controller("Rotation", degrees, 0.0f, height);
			m_RectTransform.Rotation = Math::Radians(degrees);
			DrawVec3Controller("Scale", m_RectTransform.Scale, 1.0f, height);

			Spacing(2);

			ImGui::Text("VERTICES");

			VertexBufferEditor(m_LineVertexBuffer);
			EndTreeNode();
		}

		ImGui::End();
	}

	void AppLayer::RasterizerEditor()
	{
		ImGui::Begin("Rasterizer Editor");
		
		ImGui::ColorEdit4("Clear Color", m_ClearColor.Data());
		ImGui::Checkbox("Lock Viewport to Window size", &m_LockToWindowSize);
		if (!m_LockToWindowSize)
		{
			ImGui::PushID("Viewport");
			{
				ImGui::Text("Viewport");
				ImGui::DragInt("X", &m_RasterizerViewport.X);
				ImGui::DragInt("Y", &m_RasterizerViewport.Y);

				int32 width = m_RasterizerViewport.Width;
				ImGui::DragInt("Width", &width);
				m_RasterizerViewport.Width = Math::Clamp(width, 0, width);

				int32 height = m_RasterizerViewport.Height;
				ImGui::DragInt("Height", &height);
				m_RasterizerViewport.Height = Math::Clamp(height, 0, height);
			}
			ImGui::PopID();

			ImGui::PushID("Scissors");
			{
				ImGui::Text("Scissors");
				ImGui::DragInt("X", &m_RasterizerScissors.X);
				ImGui::DragInt("Y", &m_RasterizerScissors.Y);

				int32 width = m_RasterizerScissors.Width;
				ImGui::DragInt("Width", &width);
				m_RasterizerScissors.Width = Math::Clamp(width, 0, width);

				int32 height = m_RasterizerScissors.Height;
				ImGui::DragInt("Height", &height);
				m_RasterizerScissors.Height = Math::Clamp(height, 0, height);
			}
			ImGui::PopID();
		}

		if (BeginTreeNode("Textures"))
		{
			TextureEditor("SMILE-EMOJI_128x128:", m_Emoji_128);
			TextureEditor("SMILE-EMOJI_32x32:", m_Emoji_32);
			TextureEditor("WALLPAPERS_256x256:", m_Wallpapers);

			EndTreeNode();
		}

		Spacing(3);

		if (BeginTreeNode("Shaders"))
		{
			ImGui::Text("QUAD_SHADER:");
			ImGui::ColorEdit4("Tint", m_QuadShader.u_Tint.Data());
			ImGui::DragFloat("Tiling", &m_QuadShader.u_Tiling, 0.05);
			ImGui::SliderInt("TextureSlot", &m_QuadShader.u_TextureSlot, 0, 2);
			ImGui::Checkbox("Enable Vertices Colors", &m_QuadShader.u_EnableVerticesColor);

			EndTreeNode();
		}

		ImGui::End();
	}

	bool AppLayer::BeginTreeNode(std::string_view label, bool defaultOpen)
	{
		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_AllowItemOverlap |
			ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_FramePadding;

		if (defaultOpen)
			flags |= ImGuiTreeNodeFlags_DefaultOpen;

		bool result = ImGui::TreeNodeEx(label.data(), flags, label.data());

		return result;
	}

	void AppLayer::EndTreeNode()
	{
		ImGui::TreePop();
	}

	void AppLayer::TextureEditor(std::string_view label, Ref<Texture> texture)
	{
		ImGui::Text(label.data());

		ImGui::PushID(label.data());

		Core::TextureWrap& wrap = texture->GetSampler().Wrap;

		std::string_view preview = TextureWrapToString(wrap);
		if (ImGui::BeginCombo("Wrap", preview.data()))
		{
			const Core::TextureWrap wrapArray[] = { Core::TextureWrap::REPEAT, Core::TextureWrap::CLAMP_TO_BORDER };

			for (uint32 i = 0; i < std::size(wrapArray); ++i)
			{
				bool isSelected = wrapArray[i] == wrap;
				if (ImGui::Selectable(TextureWrapToString(wrapArray[i]).data(), isSelected))
					wrap = wrapArray[i];

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		Core::TextureFilter& filter = texture->GetSampler().Filter;

		preview = TextureFilterToString(filter);
		if (ImGui::BeginCombo("Filter", preview.data()))
		{
			const Core::TextureFilter filterArray[] = { Core::TextureFilter::LINEAR, Core::TextureFilter::NEAREST };

			for (uint32 i = 0; i < std::size(filterArray); ++i)
			{
				bool isSelected = filterArray[i] == filter;
				if (ImGui::Selectable(TextureFilterToString(filterArray[i]).data(), isSelected))
					filter = filterArray[i];

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::PopID();

		Spacing(2);
	}

	void AppLayer::VertexBufferEditor(Ref<VertexBuffer> vertexBuffer)
	{
		std::vector<Vertex>& vertices = vertexBuffer->GetData();

		for (uint32 i = 0; i < vertices.size(); ++i)
		{
			ImGui::PushID(i);

			ImGui::DragFloat2("Position", vertices[i].Position.Data(), 0.05f);
			ImGui::ColorEdit4("Color", vertices[i].Color.Data());
			ImGui::DragFloat2("TexCoords", vertices[i].TexCoords.Data(), 0.05f);

			ImGui::PopID();

			Spacing(3);
		}
	}
}
