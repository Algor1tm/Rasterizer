#include "Buffers.h"


namespace Raster
{
	Ref<IndexBuffer> IndexBuffer::Create(uint32* indices, uint32 count)
	{
		auto indexBuffer = Core::CreateRef<IndexBuffer>();
		indexBuffer->m_Buffer = std::vector<uint32>(indices, indices + count);

		return indexBuffer;
	}

	Ref<VertexBuffer> VertexBuffer::Create(Vertex* vertices, uint32 count)
	{
		auto vertexBuffer = Core::CreateRef<VertexBuffer>();
		vertexBuffer->m_Buffer = std::vector<Vertex>(vertices, vertices + count);
		
		return vertexBuffer;
	}
}
