#include "AppLayer.h"

#include "Shader.h"

#include <Core/Application.h>
#include <ImGui/imgui.h>



namespace Raster
{
	class MyShader
	{
	public:
		static Interpolators VertexShader(const VertexShaderInput& input, Vector4& outPosition)
		{
			Interpolators output;
			output.Color = input.Color;

			outPosition = input.Position;

			return output;
		}

		static Vector4 FragmentShader(const Interpolators& input)
		{
			return input.Color;
		}
	};

	AppLayer::AppLayer()
		: Layer("RasterizerLayer")
	{
		m_SwapChain = SwapChain::Create(1, 1);
		m_RenderTarget = RenderTarget::Create(1, 1);
		
		RasterizerState state;
		m_Rasterizer = Rasterizer::Create(state);

		float vertices[] = { 0.f,  -0.5f, 0,   1, 0, 0, 1, 
							-0.6f, 0.4f, 0,    0, 1, 0, 1, 
							 0.6f,  0.4f, 0,   0, 0, 1, 1, };
		
		uint32 indices[] = { 0, 1, 2 };

		VertexBufferCreateInfo triangleInfo;
		triangleInfo.Data = (Vertex*)vertices;
		triangleInfo.Size = std::size(vertices) / 7;
		triangleInfo.Primitives = PrimitiveType::TRIANGLE_LIST;
		triangleInfo.IndexBuffer = IndexBuffer::Create(indices, std::size(indices));

		m_VertexBuffer = VertexBuffer::Create(triangleInfo);

		float lineVertices[] = { -0.5f, -0.5f, 0,   1, 0, 0, 1,
								  0.5f,  0.5f, 0,   0, 0, 1, 1 };

		uint32 lineIndices[] = { 0, 1 };

		VertexBufferCreateInfo lineInfo;
		lineInfo.Data = (Vertex*)lineVertices;
		lineInfo.Size = std::size(lineVertices) / 7;
		lineInfo.Primitives = PrimitiveType::LINE_LIST;
		lineInfo.IndexBuffer = IndexBuffer::Create(lineIndices, std::size(lineIndices));

		m_LineVertexBuffer = VertexBuffer::Create(lineInfo);
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
		geometryPass.VertexShader = &MyShader::VertexShader;
		geometryPass.FragmentShader = &MyShader::FragmentShader;

		m_Rasterizer->BeginRenderPass(geometryPass);
		m_RenderTarget->Clear({ 0.8f, 0.3f, 0.2f, 1.f });

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

		m_Rasterizer->DrawElements(m_VertexBuffer);
		m_Rasterizer->DrawElements(m_LineVertexBuffer);

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

		ImGui::Image(m_SwapChain->GetNativeTextureHandle(), { (float)m_RenderTarget->GetWidth(), (float)m_RenderTarget->GetHeight() });

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Settings");

		ImGui::Text("FrameTime: %.3f", 1.f / m_FrameTime.AsSeconds());

		bool oldVSync = Core::Application::Get().GetWindow().IsVSync();
		bool newVSync = oldVSync;
		ImGui::Checkbox("VSync", &newVSync);

		if (oldVSync != newVSync)
			Core::Application::Get().GetWindow().SetVSync(newVSync);

		std::vector<Vertex>& vertices = m_VertexBuffer->GetData();

		ImGui::Text("TRIANGLE");
		DrawVertexEditor("Point0", vertices[0]);
		DrawVertexEditor("Point1", vertices[1]);
		DrawVertexEditor("Point2", vertices[2]);

		std::vector<Vertex>& lineVertices = m_LineVertexBuffer->GetData();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("LINE");
		DrawVertexEditor("LinePoint0", lineVertices[0]);
		DrawVertexEditor("LinePoint1", lineVertices[1]);

		ImGui::End();
	}

	void AppLayer::DrawVertexEditor(std::string_view label, Vertex& vertex)
	{
		ImGui::PushID(label.data());

		ImGui::Text(label.data());
		ImGui::DragFloat2("Position", vertex.Position.Data(), 0.05);
		ImGui::ColorEdit4("Color", vertex.Color.Data());

		ImGui::PopID();
		ImGui::Spacing();
	}

	bool AppLayer::OnWindowResize(Core::WindowResizeEvent& event)
	{
		return false;
	}

	void AppLayer::OnEvent(Core::Event& event)
	{
		Core::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Core::WindowResizeEvent>(APP_BIND_EVENT_FN(OnWindowResize));
	}
}
