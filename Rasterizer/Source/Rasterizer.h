#pragma once

#include "Buffers.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "Shader.h"

#include <Core/Color.h>
#include <Math/Vector.h>

#include <array>


namespace Raster
{
	struct RasterizerState
	{
		// antialising
	};

	struct Rect
	{
		int32 X;
		int32 Y;
		uint32 Width;
		uint32 Height;
	};

	using Triangle = std::array<Vector2i, 3>;

	enum ClearBit
	{
		CLEAR_NONE_BIT = BIT(0),
		CLEAR_COLOR_BIT = BIT(1),
		CLEAR_DEPTH_BIT = BIT(2),
		CLEAR_STENCIL_BIT = BIT(3)
	};

	struct RenderPass
	{
		Core::LinearColor ClearColor;
		ClearBit ClearBit;
		Ref<RenderTarget> OutputRenderTarget;
		Shader* Shader;
		VertexShader VertexShader;
		FragmentShader FragmentShader;
		Rect Viewport;
		Rect Scissors;
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
		void ComputeRenderArea();
		void Clear();

		void ExecuteGraphicsPipeline();

		bool NextPrimitive();
		void VertexShading();
		void Clipping();

		void Rasterize();

		void ProcessPixel(int32 x, int32 y);

		void FragmentShading();
		void Blending();
		void RecordPixel();

		void CleanUp();

	private:
		Vector2i LineLineSegmentIntersection(Vector2i linepoint, Vector2i linenormal, Vector2i p1, Vector2i p2);
		uint8 ClipTriangleAgainstLine(Vector2i linepoint, Vector2i linenormal, const Triangle& inTri, Triangle& outTri1, Triangle& outTri2);
		bool ClipLineSegmentAgainstLine(Vector2i linepoint, Vector2i linenormal, Vector2i& p1, Vector2i& p2);

		void RasterizeTriangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2);
		void RasterizeLine(const Vector2i& p0, const Vector2i& p1);

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

			Rect RenderArea;

			uint32 NextPrimitiveIndex = 0;

			std::vector<Vertex> Vertexes;
			std::vector<Vector2> ClipSpacePositions;	// [(-1, -1), (1, 1)]
			std::vector<Vector2i> ScreenSpacePositions; // [(0, 0); (width, height)]
			std::vector<Vector2i> ClippedScreenSpacePositions; // up to 5 triangles or 1 line

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
