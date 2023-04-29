#include "Buffers.h"


namespace Raster
{
	Ref<IndexBuffer> IndexBuffer::Create(uint32* indices, uint32 count)
	{
		auto indexBuffer = Core::CreateRef<IndexBuffer>();
		indexBuffer->m_Buffer = std::vector<uint32>(indices, indices + count);

		return indexBuffer;
	}

	Ref<VertexBuffer> VertexBuffer::Create(const VertexBufferCreateInfo& createInfo)
	{
		auto vertexBuffer = Core::CreateRef<VertexBuffer>();
		vertexBuffer->m_Buffer = std::vector<Vertex>(createInfo.Data, createInfo.Data + createInfo.Size);
		vertexBuffer->m_Primitives = createInfo.Primitives;
		vertexBuffer->m_IndexBuffer = createInfo.IndexBuffer;

		return vertexBuffer;
	}

	void VertexBuffer::SetData(const std::vector<Vertex>& data)
	{
		m_Buffer = data;
	}
}
