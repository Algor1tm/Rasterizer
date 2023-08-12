#pragma once

#include <Math/Vector.h>
#include <Core/Color.h>


namespace Raster
{
	enum class PrimitiveType
	{
		TRIANGLE_LIST, LINE_LIST
	};

	struct Vertex
	{
		Vector3 Position;
		Core::LinearColor Color;
		Vector2 TexCoords;
	};

	class IndexBuffer
	{
	public:
		static Core::Ref<IndexBuffer> Create(uint32* indices, uint32 count);
		const std::vector<uint32> Data() const { return m_Buffer; }

	private:
		std::vector<uint32> m_Buffer;
	};

	struct VertexBufferCreateInfo
	{
		Vertex* Data;
		uint32 Size;
		PrimitiveType Primitives;
		Ref<IndexBuffer> IndexBuffer;
	};

	class VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create(const VertexBufferCreateInfo& createInfo);

		std::vector<Vertex>& GetData() { return m_Buffer; }
		const std::vector<Vertex>& GetData() const { return m_Buffer; }

		void SetData(const std::vector<Vertex>& data);

		const std::vector<Vertex> Data() const { return m_Buffer; }
		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
		PrimitiveType GetPrimitiveType() const { return m_Primitives; }

	private:
		std::vector<Vertex> m_Buffer;
		PrimitiveType m_Primitives;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}
