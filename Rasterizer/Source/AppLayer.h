#pragma once

#include "Buffers.h"
#include "Camera.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "Rasterizer.h"
#include "Shader.h"
#include "SwapChain.h"

#include <Core/Layer.h>
#include <Input/ApplicationEvent.h>
#include <Math/Matrix.h>
#include <Math/Transforms.h>
#include <Math/Quaternion.h>


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

			Vertex_Position = Vector4(input.Position, 1) * u_MVPMatrix;

			return output;
		}

		Vector4 FragmentShader(const Interpolators& input)
		{
			Ref<Texture> texture = GetTexture(u_TextureSlot);
			Vector4 texColor = texture->Sample(input.TexCoords * u_Tiling);

			Vector4 result = texColor * u_Tint;

			if (u_EnableVerticesColor)
				result *= input.Color;

			return result;
		}

	public:
		// Vertex Shader Uniforms
		Matrix4 u_MVPMatrix = Matrix4::Identity();

		// Fragment Shader Uniforms
		Vector4 u_Tint = Vector4(1, 1, 1, 1);
		float u_Tiling = 1.f;
		int32 u_TextureSlot = 0;
		bool u_EnableVerticesColor = false;
	};

	struct Transform
	{
		Vector3 Translation = { 0.f, 0.f, 0.f };
		Vector3 Rotation = { 0.f, 0.f, 0.f };
		Vector3 Scale = { 1.f, 1.f, 1.f };

		Transform() = default;
		Transform(const Vector3& position)
			: Translation(position) {}

		Matrix4 AsMatrix() const
		{
			return Math::ConstructTransform(Translation, Scale, Quaternion(Rotation));
		}
	};

	class AppLayer : public Core::Layer
	{
	public:
		AppLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGuiRender() override;
		virtual void OnUpdate(Core::Time frameTime) override;
		virtual void OnEvent(Core::Event& event) override;

	private:
		void Render();

		void InitVertexBuffers();

		void GeneralEditor();
		void MeshesEditor();
		void RasterizerEditor();

		bool BeginTreeNode(std::string_view label, bool defaultOpen = false);
		void EndTreeNode();

		void TextureEditor(std::string_view label, Ref<Texture> texture);
		void VertexBufferEditor(Ref<VertexBuffer> vertexBuffer);

	private:
		Core::Time m_FrameTime;

		OrthographicCamera m_Camera;

		MyShader m_Shader;
		Ref<RenderTarget> m_RenderTarget;
		Ref<Rasterizer> m_Rasterizer;
		Ref<SwapChain> m_SwapChain;

		Rect m_RasterizerViewport;
		Rect m_RasterizerScissors;
		bool m_LockToWindowSize = true;

		Ref<VertexBuffer> m_QuadVertexBuffer;
		Ref<VertexBuffer> m_LineVertexBuffer;

		Vector2 m_ViewportSize = { 1200, 720 };
		bool m_IsViewportHovered = false;

		Core::LinearColor m_ClearColor = { 0.8f, 0.3f, 0.2f, 1.f };

		Ref<Texture> m_Emoji_128;
		Ref<Texture> m_Emoji_32;
		Ref<Texture> m_Wallpapers;

		Transform m_QuadTransform;
	};
}
