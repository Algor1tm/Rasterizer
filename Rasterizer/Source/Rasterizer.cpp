#include "Rasterizer.h"


namespace Raster
{
	Ref<Rasterizer> Rasterizer::Create(const RasterizerState& state)
	{
		Ref<Rasterizer> rasterizer = Core::CreateRef<Rasterizer>();
		rasterizer->m_State = state;

		return rasterizer;
	}

	void Rasterizer::BeginRenderPass(RenderTarget* target)
	{
		marget = target;
	}

	void Rasterizer::EndRenderPass()
	{

	}

	void Rasterizer::DrawTriangles(Ref<VertexBuffer> vertices, Ref<IndexBuffer> indices)
	{
		if (marget == nullptr)
			return;

		const std::vector<uint32>& indicesData = indices->Data();
		const std::vector<Vertex>& verticesData = vertices->Data();

		for (uint32 i = 0; i < indicesData.size(); i += 3)
		{
			const Vector3& p0f = verticesData[i].Position;
			const Vector3& p1f = verticesData[i + 1].Position;
			const Vector3& p2f = verticesData[i + 2].Position;

			Vector3 pArray[] = { p0f, p1f, p2f };
			std::sort(std::begin(pArray), std::end(pArray), [](const auto& left, const auto& right) {return left.y < right.y; });

			Vector2i p0 = { pArray[0].x * marget->GetWidth(), pArray[0].y * marget->GetHeight() };
			Vector2i p1 = { pArray[1].x * marget->GetWidth(), pArray[1].y * marget->GetHeight() };
			Vector2i p2 = { pArray[2].x * marget->GetWidth(), pArray[2].y * marget->GetHeight() };

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
			Pixel& pixel = marget->Get(x, y);

			pixel.Red = 255;
			pixel.Green = 255;
			pixel.Blue = 255;
			pixel.Alpha = 255;
		}
	}
}
