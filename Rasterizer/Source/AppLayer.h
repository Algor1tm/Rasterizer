#pragma once

#include <Core/Layer.h>
#include <Input/ApplicationEvent.h>

#include "Buffers.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "Rasterizer.h"
#include "Shader.h"
#include "SwapChain.h"


namespace Raster
{
	class MyShader: public Shader
	{
	public:
		Interpolators VertexShader(const VertexShaderInput& input)
		{
			Interpolators output;
			output.Color = input.Color;
			output.TexCoords = input.TexCoords;

			Vertex_Position = input.Position;

			return output;
		}

		Vector4 FragmentShader(const Interpolators& input)
		{
			Ref<Texture> texture = GetTexture(0);
			Vector4 texColor = texture->Sample(input.TexCoords);

			return texColor;
		}
	};

	class AppLayer : public Core::Layer
	{
	public:
		AppLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnImGuiRender() override;
		void OnUpdate(Core::Time frameTime) override;
		void OnEvent(Core::Event& event) override;

	private:
		bool OnWindowResize(Core::WindowResizeEvent& event);
		void DrawVertexEditor(std::string_view label, Vertex& vertex);

	private:
		Core::Time m_FrameTime;

		MyShader m_Shader;
		Ref<RenderTarget> m_RenderTarget;
		Ref<Rasterizer> m_Rasterizer;
		Ref<SwapChain> m_SwapChain;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<VertexBuffer> m_LineVertexBuffer;

		Vector2 m_ViewportSize = { 1200, 720 };

		Ref<Texture> m_Emoji;
		Ref<Texture> m_Wallpapers;
	};
}
