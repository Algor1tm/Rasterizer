#pragma once

#include "Buffers.h"
#include "RenderTarget.h"

#include <Math/Vector.h>


namespace Raster
{
	struct RasterizerState
	{
		// blending, depth, antialising
	};

	class Rasterizer
	{
	public:
		static Ref<Rasterizer> Create(const RasterizerState& state);

		void BeginRenderPass(RenderTarget* target);
		void EndRenderPass();

		void DrawLines(Ref<VertexBuffer> vertices, Ref<IndexBuffer> indices);
		void DrawTriangles(Ref<VertexBuffer> vertices, Ref<IndexBuffer> indices);

	private:
		void FillBottomFlatTriangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2); // p1 = p2
		void FillTopFlatTriangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2); // p0 = p1

		void FillRow(int32 x0, int32 x1, int32 y);
		void FillColumn(int32 y0, int32 y1, int32 x);

	private:
		RasterizerState m_State;
		RenderTarget* marget = nullptr;
	};
}
