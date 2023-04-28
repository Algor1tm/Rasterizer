#include "Rasterizer.h"


namespace Raster
{
	Ref<Rasterizer> Rasterizer::Create(const RasterizerState& state)
	{
		Ref<Rasterizer> rasterizer = Core::CreateRef<Rasterizer>();
		rasterizer->m_State = state;

		return rasterizer;
	}

	void Rasterizer::BeginRenderPass(const RenderPass& renderPass)
	{
		m_RenderPass = renderPass;
	}

	void Rasterizer::EndRenderPass()
	{
		m_RenderPass = RenderPass();
		m_DrawCallInfo = DrawCallInfo();
	}

	void Rasterizer::DrawTriangles(Ref<VertexBuffer> vertices, Ref<IndexBuffer> indices)
	{
		if (m_RenderPass.OutputRenderTarget == nullptr)
			return;
		
		if (vertices == nullptr || indices == nullptr)
			return;

		m_DrawCallInfo.InputVertexBuffer = vertices->Data();
		m_DrawCallInfo.InputIndexBuffer = indices->Data();

		ExecuteGraphicsPipeline();
	}

	void Rasterizer::ExecuteGraphicsPipeline()
	{
		while(NextPrimitive())
		{
			VertexShading();
			Clipping();

			PrepareRasterization();

			while (!m_DrawCallInfo.FinishPrimitiveRasterization)
			{
				Rasterization();
				//DepthTest();
				FragmentShading();
				//Blending();
				RecordPixel();
			}
		}

		CleanUp();
	}

	bool Rasterizer::NextPrimitive()
	{
		uint32 vertexCount = 0;
		if (m_DrawCallInfo.InputPrimitives == Primitives::TRIANGLE_LIST)
			vertexCount = 3;

		if (m_DrawCallInfo.NextPrimitiveIndex + vertexCount <= m_DrawCallInfo.InputIndexBuffer.size())
		{
			m_DrawCallInfo.Vertexes.resize(vertexCount);

			for (uint32 i = 0; i < vertexCount; ++i)
			{
				uint32 vertexIndex = m_DrawCallInfo.InputIndexBuffer[m_DrawCallInfo.NextPrimitiveIndex + i];
				m_DrawCallInfo.Vertexes[i] = m_DrawCallInfo.InputVertexBuffer[vertexIndex];
			}

			m_DrawCallInfo.NextPrimitiveIndex += vertexCount;
			return true;
		}

		return false;
	}

	void Rasterizer::VertexShading()
	{
		m_DrawCallInfo.VertexInterpolators.resize(m_DrawCallInfo.Vertexes.size());
		m_DrawCallInfo.ClipSpacePositions.resize(m_DrawCallInfo.Vertexes.size());
		std::vector<Vector4> homogeneousPositions(m_DrawCallInfo.Vertexes.size());

		VertexShaderInput input;

		for (uint32 i = 0; i < m_DrawCallInfo.Vertexes.size(); ++i)
		{
			const Vertex& vertex = m_DrawCallInfo.Vertexes[i];
			input.Position = vertex.Position;
			input.Color = vertex.Color;

			m_DrawCallInfo.VertexInterpolators[i] = m_RenderPass.VertexShader(input, homogeneousPositions[i]);

			m_DrawCallInfo.ClipSpacePositions[i] = Vector2(homogeneousPositions[i] / homogeneousPositions[i].w);
		}
	}

	void Rasterizer::Clipping()
	{
		// TODO: Clipping, Viewport, Scissors

		m_DrawCallInfo.ScreenSpacePositions.resize(m_DrawCallInfo.ClipSpacePositions.size());

		for (uint32 i = 0; i < m_DrawCallInfo.ClipSpacePositions.size(); ++i)
		{
			Vector2 viewSpace = (m_DrawCallInfo.ClipSpacePositions[i] + 1.f) / 2.f;
			m_DrawCallInfo.ScreenSpacePositions[i].x = viewSpace.x * m_RenderPass.OutputRenderTarget->GetWidth();
			m_DrawCallInfo.ScreenSpacePositions[i].y = viewSpace.y * m_RenderPass.OutputRenderTarget->GetHeight();
		}

		if (m_DrawCallInfo.ScreenSpacePositions.size() == 0)
			m_DrawCallInfo.FinishPrimitiveRasterization = true;
	}

	void Rasterizer::PrepareRasterization()
	{
		if (m_DrawCallInfo.FinishPrimitiveRasterization)
			return;

		std::sort(m_DrawCallInfo.ScreenSpacePositions.begin(), m_DrawCallInfo.ScreenSpacePositions.end(),
			[](const auto& left, const auto& right) {return left.y < right.y; });
	}

	void Rasterizer::Rasterization()
	{
		switch (m_DrawCallInfo.InputPrimitives)
		{
		case Primitives::TRIANGLE_LIST:
		{
			RasterizeTriangle();
			break;
		}
		}
	}

	void Rasterizer::FragmentShading()
	{
		Vector4 color = m_RenderPass.FragmentShader(m_DrawCallInfo.PixelInterpolators);
		m_DrawCallInfo.ResultColor = color;
	}

	void Rasterizer::RecordPixel()
	{
		Vector4 color = m_DrawCallInfo.ResultColor;
		auto [x, y] = m_DrawCallInfo.PixelCoords;

		Pixel& pixel = m_RenderPass.OutputRenderTarget->Get(x, y);
		pixel.Red = color.x;
		pixel.Green = color.y;
		pixel.Blue = color.z;
		pixel.Alpha = color.w;
	}

	void Rasterizer::CleanUp()
	{
		m_DrawCallInfo.Vertexes.clear();
		m_DrawCallInfo.ClipSpacePositions.clear();
		m_DrawCallInfo.ScreenSpacePositions.clear();
		m_DrawCallInfo.VertexInterpolators.clear();
	}

	void Rasterizer::RasterizeTriangle()
	{
		const Vector2i& p0 = m_DrawCallInfo.ScreenSpacePositions[0];
		const Vector2i& p1 = m_DrawCallInfo.ScreenSpacePositions[1];
		const Vector2i& p2 = m_DrawCallInfo.ScreenSpacePositions[2];

		if (p1.y == p2.y)
		{
			FillBottomFlatTriangle(p0, p1, p2);
		}
		else if (p0.y == p1.y)
		{
			FillTopFlatTriangle(p0, p1, p2);
		}
		else
		{
			Vector2i p3 = { (p0.x + ((float)(p1.y - p0.y) / (float)(p2.y - p0.y)) * (p2.x - p0.x)), p1.y };
			FillBottomFlatTriangle(p0, p1, p3);
			FillTopFlatTriangle(p1, p3, p2);
		}
	}

	void Rasterizer::TrilinearInterpolation()
	{

	}

	// TODO: rework 3 functions below

	void Rasterizer::FillBottomFlatTriangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2)
	{
		float invslope1 = (p1.x - p0.x) / (float)(p1.y - p0.y);
		float invslope2 = (p2.x - p0.x) / (float)(p2.y - p0.y);

		float curx1 = p0.x;
		float curx2 = p0.x;

		for (int scanlineY = p0.y; scanlineY <= p1.y; scanlineY++)
		{
			FillRow(curx1, curx2, scanlineY);
			curx1 += invslope1;
			curx2 += invslope2;
		}
	}

	void Rasterizer::FillTopFlatTriangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2)
	{
		float invslope1 = (p2.x - p0.x) / (float)(p2.y - p0.y);
		float invslope2 = (p2.x - p1.x) / (float)(p2.y - p1.y);

		float curx1 = p2.x;
		float curx2 = p2.x;

		for (int scanlineY = p2.y; scanlineY > p0.y; scanlineY--)
		{
			FillRow(curx1, curx2, scanlineY);
			curx1 -= invslope1;
			curx2 -= invslope2;
		}
	}
	
	void Rasterizer::FillRow(int32 x0, int32 x1, int32 y)
	{
		for (int32 x = Math::Min(x0, x1); x < Math::Max(x0, x1); ++x)
		{
			m_DrawCallInfo.PixelCoords = { x, y };
		}
	}
}
