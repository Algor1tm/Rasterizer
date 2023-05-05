#pragma once

#include "Buffers.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "Shader.h"

#include <Math/Vector.h>


namespace Raster
{
	struct RasterizerState
	{
		// antialising
	};

	struct RenderPass
	{
		Ref<RenderTarget> OutputRenderTarget;
		Shader* Shader;
		VertexShader VertexShader;
		FragmentShader FragmentShader;
	};

	class Rasterizer
	{
	public:
		static Ref<Rasterizer> Create(const RasterizerState& state);

		void BeginRenderPass(const RenderPass& renderPass);
		void EndRenderPass();

		void DrawElements(Ref<VertexBuffer> vertices);

		void BindTexture(Ref<Texture> texture, uint8 slot) { m_TextureSlots[slot] = texture; }

	private:
		void ExecuteGraphicsPipeline();

		bool NextPrimitive();
		void VertexShading();
		void Clipping();

		void Rasterize();

		void ProcessPixel();

		void FragmentShading();
		void RecordPixel();

		void CleanUp();

	private:
		void RasterizeTriangle();
		void RasterizeLine();

		void TrilinearInterpolation();
		void LinearInterpolation();

		void FillBottomFlatTriangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2); // p1 = p2
		void FillTopFlatTriangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2); // p0 = p1

		void FillRow(int32 x0, int32 x1, int32 y);

	private:
		struct DrawCallInfo
		{
			std::vector<Vertex> InputVertexBuffer;
			std::vector<uint32> InputIndexBuffer;
			PrimitiveType InputPrimitives = PrimitiveType::TRIANGLE_LIST;

			uint32 NextPrimitiveIndex = 0;

			std::vector<Vertex> Vertexes;
			std::vector<Vector2> ClipSpacePositions;	// [(-1, -1), (1, 1)]
			std::vector<Vector2i> ScreenSpacePositions; // [(0, 0); (width, height)]

			std::vector<Interpolators> VertexInterpolators;

			Vector2i PixelCoords;
			Interpolators PixelInterpolators;
			Vector4 PixelColor;
		};

	private:
		RasterizerState m_State;
		RenderPass m_RenderPass;

		DrawCallInfo m_DrawCallInfo;

		std::unordered_map<uint8, Ref<Texture>> m_TextureSlots;
	};
}
