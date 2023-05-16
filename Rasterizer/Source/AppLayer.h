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
			Vector4 texColor = texture->Sample(input.TexCoords * Tiling);

			Vector4 result = texColor;

			if (EnableVerticesColor)
				result *= input.Color;

			return result;
		}

	public:
		float Tiling = 1.f;
		bool EnableVerticesColor = false;
	};

	class AppLayer : public Core::Layer
	{
	public:
		AppLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnImGuiRender() override;
		void OnUpdate(Core::Time frameTime) override;

	private:
		void InitVertexBuffers();

		void GeneralEditor();
		void MeshesEditor();
		void RasterizerEditor();

		bool BeginTreeNode(std::string_view label, bool defaultOpen = false);
		void EndTreeNode();

		void TextureEditor(Ref<Texture> texture);
		void VertexBufferEditor(Ref<VertexBuffer> vertexBuffer);

	private:
		Core::Time m_FrameTime;

		MyShader m_Shader;
		Ref<RenderTarget> m_RenderTarget;
		Ref<Rasterizer> m_Rasterizer;
		Ref<SwapChain> m_SwapChain;

		Ref<VertexBuffer> m_QuadVertexBuffer;
		Ref<VertexBuffer> m_LineVertexBuffer;

		Vector2 m_ViewportSize = { 1200, 720 };
		Core::LinearColor m_ClearColor = { 0.8f, 0.3f, 0.2f, 1.f };

		Ref<Texture> m_Emoji;
		Ref<Texture> m_Wallpapers;
	};
}
