#include "Rasterizer.h"

#include <list>


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

		ComputeRenderArea();
		Clear();
	}

	void Rasterizer::EndRenderPass()
	{
		m_RenderPass = RenderPass();
		m_DrawCallInfo = DrawCallInfo();
	}

	void Rasterizer::DrawElements(Ref<VertexBuffer> vertices)
	{
		if (m_RenderPass.OutputRenderTarget == nullptr)
			return;

		// Input Assembler ;)
		m_DrawCallInfo.InputVertexBuffer = vertices->Data();
		m_DrawCallInfo.InputIndexBuffer = vertices->GetIndexBuffer()->Data();
		m_DrawCallInfo.InputPrimitives = vertices->GetPrimitiveType();
		
		m_RenderPass.Shader->m_TextureSlots = &m_TextureSlots;

		ExecuteGraphicsPipeline();
	}

	void Rasterizer::ExecuteGraphicsPipeline()
	{
		while(NextPrimitive())
		{
			VertexShading();
			//Culling();
			Clipping();

			Rasterize();
		}

		CleanUp();
	}

	void Rasterizer::ComputeRenderArea()
	{
		m_DrawCallInfo.RenderArea.X = Math::Max(0, m_RenderPass.Viewport.X, m_RenderPass.Scissors.X);
		m_DrawCallInfo.RenderArea.Y = Math::Max(0, m_RenderPass.Viewport.Y, m_RenderPass.Scissors.Y);

		m_DrawCallInfo.RenderArea.Width = Math::Min(m_RenderPass.Scissors.Width, m_RenderPass.Viewport.Width, m_RenderPass.OutputRenderTarget->GetWidth());
		m_DrawCallInfo.RenderArea.Height = Math::Min(m_RenderPass.Scissors.Height, m_RenderPass.Viewport.Height, m_RenderPass.OutputRenderTarget->GetHeight());
	}

	void Rasterizer::Clear()
	{
		Pixel zeroPixel = Pixel(0, 0, 0, 0);
		auto& pixels = m_RenderPass.OutputRenderTarget->ReadAllPixels();
		std::fill(pixels.begin(), pixels.end(), zeroPixel);

		Pixel clearPixel = Pixel(m_RenderPass.ClearColor);
		for (uint32 y = m_DrawCallInfo.RenderArea.Y; y < m_DrawCallInfo.RenderArea.Height; ++y)
		{
			for (uint32 x = m_DrawCallInfo.RenderArea.X; x < m_DrawCallInfo.RenderArea.Width; ++x)
			{
				m_RenderPass.OutputRenderTarget->Get(x, y) = clearPixel;
			}
		}
	}

	bool Rasterizer::NextPrimitive()
	{
		uint32 vertexCount = 0;
		if (m_DrawCallInfo.InputPrimitives == PrimitiveType::TRIANGLE_LIST)
			vertexCount = 3;

		if (m_DrawCallInfo.InputPrimitives == PrimitiveType::LINE_LIST)
			vertexCount = 2;

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

		VertexShaderInput input;

		for (uint32 i = 0; i < m_DrawCallInfo.Vertexes.size(); ++i)
		{
			const Vertex& vertex = m_DrawCallInfo.Vertexes[i];
			input.Position = vertex.Position;
			input.Color = vertex.Color;
			input.TexCoords = vertex.TexCoords;

			m_DrawCallInfo.VertexInterpolators[i] = m_RenderPass.VertexShader(input);

			Vector4 homogeneousPosition = m_RenderPass.Shader->Vertex_Position;
			m_DrawCallInfo.ClipSpacePositions[i] = Vector2(homogeneousPosition / homogeneousPosition.w);
		}
	}

	void Rasterizer::Clipping()
	{
		m_DrawCallInfo.ScreenSpacePositions.resize(m_DrawCallInfo.ClipSpacePositions.size());

		for (uint32 i = 0; i < m_DrawCallInfo.ClipSpacePositions.size(); ++i)
		{
			Vector2 viewSpace = (m_DrawCallInfo.ClipSpacePositions[i] + 1.f) / 2.f;

			m_DrawCallInfo.ScreenSpacePositions[i].x = m_RenderPass.Viewport.X + viewSpace.x * m_RenderPass.Viewport.Width;
			m_DrawCallInfo.ScreenSpacePositions[i].y = m_RenderPass.Viewport.Y + viewSpace.y * m_RenderPass.Viewport.Height;
		}

		int32 clipStartX = m_DrawCallInfo.RenderArea.X;
		int32 clipStartY = m_DrawCallInfo.RenderArea.Y;

		int32 clipEndX = clipStartX + m_DrawCallInfo.RenderArea.Width - 1;
		int32 clipEndY = clipStartY + m_DrawCallInfo.RenderArea.Height - 1;

		if (m_DrawCallInfo.InputPrimitives == PrimitiveType::TRIANGLE_LIST)
		{
			Triangle tri;
			tri[0] = m_DrawCallInfo.ScreenSpacePositions[0];
			tri[1] = m_DrawCallInfo.ScreenSpacePositions[1];
			tri[2] = m_DrawCallInfo.ScreenSpacePositions[2];

			Triangle clipped[2];
			std::list<Triangle> trianglesList;

			trianglesList.push_back(tri);
			uint32 newTriangles = 1;

			for (uint8 p = 0; p < 4; ++p)
			{
				uint8 trisToAdd = 0;
				while(newTriangles > 0)
				{
					Triangle test = trianglesList.front();
					trianglesList.pop_front();
					newTriangles--;

					switch (p)
					{
					case 0: trisToAdd = ClipTriangleAgainstLine({ clipStartX, clipStartY }, { 0, 1  }, test, clipped[0], clipped[1]); break;
					case 1: trisToAdd = ClipTriangleAgainstLine({ clipStartX, clipEndY   }, { 0, -1 }, test, clipped[0], clipped[1]); break;
					case 2: trisToAdd = ClipTriangleAgainstLine({ clipStartX, clipStartY }, { 1, 0  }, test, clipped[0], clipped[1]); break;
					case 3: trisToAdd = ClipTriangleAgainstLine({ clipEndX,   clipStartY }, { -1, 0 }, test, clipped[0], clipped[1]); break;
					}

					for (uint8 i = 0; i < trisToAdd; ++i)
					{
						trianglesList.push_back(clipped[i]);
					}
				}
				newTriangles = trianglesList.size();
			}

			m_DrawCallInfo.ClippedScreenSpacePositions.resize(trianglesList.size() * 3);
			uint32 i = 0;
			for (const auto& tri: trianglesList)
			{
				m_DrawCallInfo.ClippedScreenSpacePositions[i] = tri[0];
				m_DrawCallInfo.ClippedScreenSpacePositions[i + 1] = tri[1];
				m_DrawCallInfo.ClippedScreenSpacePositions[i + 2] = tri[2];
				i += 3;
			}
		}
		else
		{
			// TODO: Lines
			m_DrawCallInfo.ClippedScreenSpacePositions = m_DrawCallInfo.ScreenSpacePositions;
		}
	}

	Vector2i Rasterizer::LineLineSegmentIntersection(Vector2i linepoint, Vector2i linenormal, Vector2i p1, Vector2i p2)
	{
		float ad = Math::Dot(p1, linenormal);
		float bd = Math::Dot(p2, linenormal);

		float plane_d = Math::Dot(linenormal, linepoint);
		float t = (plane_d - ad) / (bd - ad);

		Vector2i result;
		result.x = (float)p1.x + float(p2.x - p1.x) * t;
		result.y = (float)p1.y + float(p2.y - p1.y) * t;

		return result;
	}

	uint8 Rasterizer::ClipTriangleAgainstLine(Vector2i linepoint, Vector2i linenormal, const Triangle& inTri, Triangle& outTri1, Triangle& outTri2)
	{
		uint8 insidePoints[3];
		uint8 insidePointCount = 0;

		uint8 outsidePoints[3];
		uint8 outsidePointCount = 0;

		float d = -Math::Dot(linenormal, linepoint);
		for (uint8 i = 0; i < 3; ++i)
		{
			float signedDistance = linenormal.x * inTri[i].x + linenormal.y * inTri[i].y + d;
			if (signedDistance >= 0)
				insidePoints[insidePointCount++] = i;
			else
				outsidePoints[outsidePointCount++] = i;
		}

		switch (insidePointCount)
		{
		case 0: return 0;
		case 3: outTri1 = inTri; return 1;
		case 1: 
		{
			outTri1[0] = inTri[insidePoints[0]];
			outTri1[1] = LineLineSegmentIntersection(linepoint, linenormal, outTri1[0], inTri[outsidePoints[0]]);
			outTri1[2] = LineLineSegmentIntersection(linepoint, linenormal, outTri1[0], inTri[outsidePoints[1]]);

			return 1;
		}
		case 2:
		{
			outTri1[0] = inTri[insidePoints[0]];
			outTri1[1] = inTri[insidePoints[1]];
			outTri1[2] = LineLineSegmentIntersection(linepoint, linenormal, outTri1[0], inTri[outsidePoints[0]]);

			outTri2[0] = inTri[insidePoints[1]];
			outTri2[1] = outTri1[2];
			outTri2[2] = LineLineSegmentIntersection(linepoint, linenormal, outTri2[0], inTri[outsidePoints[0]]);

			return 2;
		}
		}

		return 0;
	}

	void Rasterizer::Rasterize()
	{
		switch (m_DrawCallInfo.InputPrimitives)
		{
		case PrimitiveType::TRIANGLE_LIST:
		{
			for (uint32 i = 0; i < m_DrawCallInfo.ClippedScreenSpacePositions.size(); i += 3)
			{
				const Vector2i& p0 = m_DrawCallInfo.ClippedScreenSpacePositions[i + 0];
				const Vector2i& p1 = m_DrawCallInfo.ClippedScreenSpacePositions[i + 1];
				const Vector2i& p2 = m_DrawCallInfo.ClippedScreenSpacePositions[i + 2];

				RasterizeTriangle(p0, p1, p2);
			}
			
			break;
		}
		case PrimitiveType::LINE_LIST:
		{
			for (uint32 i = 0; i < m_DrawCallInfo.ClippedScreenSpacePositions.size(); i += 2)
			{
				const Vector2i& p0 = m_DrawCallInfo.ClippedScreenSpacePositions[i + 0];
				const Vector2i& p1 = m_DrawCallInfo.ClippedScreenSpacePositions[i + 1];

				RasterizeLine(p0, p1);
			}

			break;
		}
		}
	}
	
	void Rasterizer::ProcessPixel(int32 x, int32 y)
	{
		m_DrawCallInfo.PixelCoords = { x, y };

		//if(!DepthTest())
		//	return;

		FragmentShading();
		Blending();
		RecordPixel();
	}

	void Rasterizer::FragmentShading()
	{
		switch (m_DrawCallInfo.InputPrimitives)
		{
		case PrimitiveType::TRIANGLE_LIST:
		{
			TrilinearInterpolation();
			break;
		}
		case PrimitiveType::LINE_LIST:
		{
			LinearInterpolation();
			break;
		}
		}

		m_RenderPass.Shader->Fragment_PixelCoords = m_DrawCallInfo.PixelCoords;

		Vector4 color = m_RenderPass.FragmentShader(m_DrawCallInfo.PixelInterpolators);

		color = Math::Clamp(color, 0.f, 1.f);
		m_DrawCallInfo.PixelColor = color;
	}

	void Rasterizer::Blending()
	{
		auto [x, y] = m_DrawCallInfo.PixelCoords;

		Vector4 dstColor = m_RenderPass.OutputRenderTarget->Get(x, y);
		Vector4 srcColor = m_DrawCallInfo.PixelColor;

		Vector4 result = srcColor * srcColor.w + dstColor * (1 - srcColor.w);

		m_DrawCallInfo.PixelColor = result;
	}

	void Rasterizer::RecordPixel()
	{
		Vector4 color = m_DrawCallInfo.PixelColor;
		auto [x, y] = m_DrawCallInfo.PixelCoords;

		m_RenderPass.OutputRenderTarget->Get(x, y) = Pixel(color);
	}

	void Rasterizer::CleanUp()
	{
		m_DrawCallInfo.Vertexes.clear();
		m_DrawCallInfo.ClipSpacePositions.clear();
		m_DrawCallInfo.ScreenSpacePositions.clear();
		m_DrawCallInfo.ClippedScreenSpacePositions.clear();
		m_DrawCallInfo.VertexInterpolators.clear();
		m_DrawCallInfo.NextPrimitiveIndex = 0;
	}

	void Rasterizer::TrilinearInterpolation()
	{
		auto TriangleArea = [](Vector2i a, Vector2i b) {return 0.5f * Math::Cross(a, b); };

		const Vector2i& p0 = m_DrawCallInfo.ScreenSpacePositions[0];
		const Vector2i& p1 = m_DrawCallInfo.ScreenSpacePositions[1];
		const Vector2i& p2 = m_DrawCallInfo.ScreenSpacePositions[2];

		const Vector2i& x = m_DrawCallInfo.PixelCoords;

		float fullArea = TriangleArea(p1 - p0, p2 - p0);
		float area0 = TriangleArea(p1 - x, p2 - x);
		float area1 = TriangleArea(p2 - x, p0 - x);
		float area2 = fullArea - area0 - area1;

		float w0 = area0 / fullArea;
		float w1 = area1 / fullArea;
		float w2 = area2 / fullArea;

		const auto& vertInterp = m_DrawCallInfo.VertexInterpolators;

		m_DrawCallInfo.PixelInterpolators.Color = w0 * vertInterp[0].Color + w1 * vertInterp[1].Color + w2 * vertInterp[2].Color;
		m_DrawCallInfo.PixelInterpolators.TexCoords = w0 * vertInterp[0].TexCoords + w1 * vertInterp[1].TexCoords + w2 * vertInterp[2].TexCoords;
	}

	void Rasterizer::LinearInterpolation()
	{
		const Vector2i& p0 = m_DrawCallInfo.ScreenSpacePositions[0];
		const Vector2i& p1 = m_DrawCallInfo.ScreenSpacePositions[1];

		const Vector2i& x = m_DrawCallInfo.PixelCoords;

		float percent = (x - p0).Length() / (p1 - p0).Length();

		const auto& vertInterp = m_DrawCallInfo.VertexInterpolators;

		m_DrawCallInfo.PixelInterpolators.Color = vertInterp[0].Color + vertInterp[1].Color * percent;
		m_DrawCallInfo.PixelInterpolators.TexCoords = vertInterp[0].TexCoords + vertInterp[1].TexCoords * percent;
	}

	void Rasterizer::RasterizeTriangle(const Vector2i& point0, const Vector2i& point1, const Vector2i& point2)
	{
		Vector2i pArray[] = { point0, point1, point2 };
		std::sort(std::begin(pArray), std::end(pArray), [](const auto& left, const auto& right) {return left.y < right.y; });

		const Vector2i& p0 = pArray[0];
		const Vector2i& p1 = pArray[1];
		const Vector2i& p2 = pArray[2];

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

	void Rasterizer::RasterizeLine(const Vector2i& p2, const Vector2i& p1)
	{
		// calculate dx & dy
		int dx = p2.x - p1.x;
		int dy = p2.y - p1.y;

		// calculate steps required for generating pixels
		int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

		// calculate increment in x & y for each steps
		float Xinc = dx / (float)steps;
		float Yinc = dy / (float)steps;

		// Put pixel for each step
		float X = p1.x;
		float Y = p1.y;
		for (int i = 0; i <= steps; i++) {
			ProcessPixel(round(X), round(Y));
			X += Xinc; // increment in x at each step
			Y += Yinc; // increment in y at each step
		}

		//int32 x1 = Math::Min(p1.x, p2.x);
		//int32 y1 = Math::Min(p1.y, p2.y);
		//int32 x2 = Math::Max(p1.x, p2.x);
		//int32 y2 = Math::Max(p1.y, p2.y);
		//
		//int32 m_new = 2 * (y2 - y1);
		//int32 slope_error_new = m_new - (x2 - x1);
		//for (int x = x1, y = y1; x <= x2; x++)
		//{
		//	ProcessPixel(x, y);
		//	// Add slope to increment angle formed
		//	slope_error_new += m_new;
		//
		//	// Slope error reached limit, time to
		//	// increment y and update slope error.
		//	if (slope_error_new >= 0) {
		//		y++;
		//		slope_error_new -= 2 * (x2 - x1);
		//	}
		//}

#if 0
		//int32 dx = Math::Abs(p2.x - p1.x);
		//int32 dy = Math::Abs(p2.y - p1.y);
		//int32 steps, k;
		//steps = dx;
		//int32 x, y, p0 = (2 * dy) - dx;
		//ProcessPixel(p1.x, p1.y);
		//x = p1.x;
		//y = p1.y;
		//for (k = 0; k < steps; k++)
		//{
		//	if (p0 < 0)
		//	{
		//		p0 = p0 + (2 * dy);
		//		x += 1;
		//	}
		//	else
		//	{
		//		p0 = p0 + (2 * dy) - (2 * dx);
		//		x += 1;
		//		y += 1;
		//	}
		//	ProcessPixel(x, y);
		//}
#else
		//int32 x1 = Math::Floor(p1.x);
		//int32 y1 = Math::Floor(p1.y);
		//int32 x2 = Math::Floor(p2.x);
		//int32 y2 = Math::Floor(p2.y);
		//int32 deltaX = Math::Abs(x2 - x1);
		//int32 deltaY = Math::Abs(y2 - y1);
		//int32 stepX = (x1 < x2) ? 1 : -1;
		//int32 stepY = (y1 < y2) ? 1 : -1;
		//float x = x1;
		//float y = y1;

		//if (deltaX == 0 && deltaY == 0) 
		//{
		//	ProcessPixel(x, y);
		//	return;
		//}

		//if (deltaX > deltaY) 
		//{
		//	int32 error = 2 * deltaY - deltaX;
		//	for (int32 i = 0; i <= deltaX; i++)
		//	{
		//		x += 0.5f;
		//		y += 0.5f;
		//		ProcessPixel(x, y);

		//		if (error > 0) 
		//		{
		//			error -= deltaX * 2;
		//			y += stepY;
		//		}

		//		x += stepX;
		//		error += deltaY * 2;
		//	}
		//}
		//else 
		//{
		//	int32 error = 2 * deltaX - deltaY;
		//	for (int32 i = 0; i <= deltaY; i++)
		//	{
		//		x += 0.5f;
		//		y += 0.5f;
		//		ProcessPixel(x, y);

		//		if (error > 0) 
		//		{
		//			error -= deltaY * 2;
		//			x += stepX;
		//		}

		//		y += stepY;
		//		error += deltaX * 2;
		//	}
		//}
#endif
//		float kx = (p1.y - p0.y) / (float)(p1.x - p0.x);
//		float ky = 1 / kx;
//
//		float x = p0.x;
//		float y = p0.y;
//
//		if (Math::Abs(kx) >= 0 && Math::Abs(kx) <= 1) 
//		{
//			for (int32 x = Math::Min(p0.x, p1.x); x < Math::Max(p0.x, p1.x); ++x)
//			{
//				y += kx;
//				ProcessPixel(x, y);
//			}
//		}
//		else 
//		{
//			for (int32 y = Math::Min(p0.y, p1.y); y < Math::Max(p0.y, p1.y); ++y)
//			{
//				x += ky;
//				ProcessPixel(x, y);
//			}
//		}
	}

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
			ProcessPixel(x, y);
		}
	}
}
