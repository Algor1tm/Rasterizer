#include "AppLayer.h"

#include <Core/Application.h>
#include <ImGui/imgui.h>


namespace Raster
{
	static std::string_view TextureWrapToString(Core::TextureWrap wrap)
	{
		switch (wrap)
		{
		case Core::TextureWrap::CLAMP_TO_BORDER: return "CLAMP_TO_BORDER";
		case Core::TextureWrap::REPEAT: return "REPEAT";
		}

		return "";
	}

	static void Spacing(uint32 size)
	{
		for (uint32 i = 0; i < size; ++i)
			ImGui::Spacing();
	}

	AppLayer::AppLayer()
		: Layer("RasterizerLayer")
	{
		m_SwapChain = SwapChain::Create(1, 1);
		m_RenderTarget = RenderTarget::Create(1, 1);
		
		RasterizerState state;
		m_Rasterizer = Rasterizer::Create(state);

		InitVertexBuffers();

		m_Emoji = Texture::Create("Assets/Textures/AwesomeFace128x128.png");
		m_Wallpapers = Texture::Create("Assets/Textures/Wallpapers256x256.jpg");
	}

	void AppLayer::InitVertexBuffers()
	{
		float vertices[] = { -0.3f, -0.3f, 0,   1, 0, 0, 1,   0.f, 0.f,
					 -0.3f,  0.3f, 0,   0, 1, 0, 1,   0.f, 1.f,
					  0.3f, -0.3f, 0,   0, 0, 1, 1,   1.f, 0.f,
					  0.3f,  0.3f, 0,   1, 0, 0, 1,   1.f, 1.f, };

		uint32 indices[] = { 0, 1, 2, 2, 1, 3 };

		VertexBufferCreateInfo triangleInfo;
		triangleInfo.Data = (Vertex*)vertices;
		triangleInfo.Size = std::size(vertices) / (sizeof(Vertex) / sizeof(float));
		triangleInfo.Primitives = PrimitiveType::TRIANGLE_LIST;
		triangleInfo.IndexBuffer = IndexBuffer::Create(indices, std::size(indices));

		m_QuadVertexBuffer = VertexBuffer::Create(triangleInfo);

		float lineVertices[] = { -0.5f, -0.5f, 0,   1, 0, 0, 1,	 0.f, 0.f,
								  0.5f,  0.5f, 0,   0, 0, 1, 1,  1.f, 1.f };

		uint32 lineIndices[] = { 0, 1 };

		VertexBufferCreateInfo lineInfo;
		lineInfo.Data = (Vertex*)lineVertices;
		lineInfo.Size = std::size(lineVertices) / (sizeof(Vertex) / sizeof(float));
		lineInfo.Primitives = PrimitiveType::LINE_LIST;
		lineInfo.IndexBuffer = IndexBuffer::Create(lineIndices, std::size(lineIndices));

		m_LineVertexBuffer = VertexBuffer::Create(lineInfo);

		Core::Application::Get().GetWindow().SetVSync(false);

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
		}

		static uint32 posx = 100;
		static uint32 posy = 100;

		RenderPass geometryPass;
		geometryPass.OutputRenderTarget = m_RenderTarget;
		geometryPass.Shader = &m_Shader;
		geometryPass.VertexShader = BIND_VERTEX_SHADER(m_Shader);
		geometryPass.FragmentShader = BIND_FRAGMENT_SHADER(m_Shader);

		m_Rasterizer->BeginRenderPass(geometryPass);
		m_RenderTarget->Clear(m_ClearColor);

		m_Rasterizer->BindTexture(m_Emoji, 0);
		m_Rasterizer->BindTexture(m_Wallpapers, 1);

		// Rect
		for (int32 x = 0; x < 100; ++x)
		{
			for (int32 y = 0; y < 100; ++y)
			{
				m_RenderTarget->Get(posx + x, posy + y) = { 255, 255, 255 };
			}
		}

		posx = (posx + 2) % (uint32)(m_RenderTarget->GetWidth() - 101);
		posy = (posy + 1) % (uint32)(m_RenderTarget->GetHeight() - 101);

		m_Rasterizer->DrawElements(m_QuadVertexBuffer);
		//m_Rasterizer->DrawElements(m_LineVertexBuffer);

		m_Rasterizer->EndRenderPass();

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

		const auto& [viewportX, viewportY] = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportX, viewportY };

		ImGui::Image(m_SwapChain->GetNativeTextureHandle(), { (float)m_RenderTarget->GetWidth(), (float)m_RenderTarget->GetHeight() }, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

		GeneralEditor();
		MeshesEditor();
		RasterizerEditor();
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
			VertexBufferEditor(m_QuadVertexBuffer);
			EndTreeNode();
		}

		if (BeginTreeNode("LINE"))
		{
			VertexBufferEditor(m_LineVertexBuffer);
			EndTreeNode();
		}

		ImGui::End();
	}

	void AppLayer::RasterizerEditor()
	{
		ImGui::Begin("Rasterizer Editor");
		
		ImGui::ColorEdit4("Clear Color", m_ClearColor.Data());

		if (BeginTreeNode("Textures"))
		{
			ImGui::Text("SMILE-EMOJI:");
			TextureEditor(m_Emoji);

			EndTreeNode();
		}

		Spacing(3);

		if (BeginTreeNode("Shaders"))
		{
			ImGui::Text("MY_SHADER:");
			ImGui::DragFloat("Tiling", &m_Shader.Tiling, 0.05);
			ImGui::Checkbox("Enable Vertices Colors", &m_Shader.EnableVerticesColor);

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

	void AppLayer::TextureEditor(Ref<Texture> texture)
	{
		Core::TextureWrap& wrap = texture->GetSampler().Wrap;

		std::string_view preview = TextureWrapToString(wrap);  // Pass in the preview value visible before opening the combo (it could be anything)
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
