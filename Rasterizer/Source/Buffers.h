#pragma once

#include <Math/Vector.h>
#include <Core/Color.h>


namespace Raster
{
	struct Vertex
	{
		Vector3 Position;
		Core::LinearColor Color;
	};


	class IndexBuffer
	{
	public:
		static Core::Ref<IndexBuffer> Create(uint32* indices, uint32 count);

		const std::vector<uint32> Data() const { return m_Buffer; }

	private:
		std::vector<uint32> m_Buffer;
	};

	class VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create(Vertex* vertices, uint32 count);

		const std::vector<Vertex> Data() const { return m_Buffer; }

	private:
		std::vector<Vertex> m_Buffer;
	};
}
