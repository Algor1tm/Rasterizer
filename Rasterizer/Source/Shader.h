#pragma once

#include <Math/Vector.h>

#include <functional>


namespace Raster
{
	struct VertexShaderInput
	{
		Vector3 Position;
		Vector4 Color;
		Vector2 TexCoords;
	};

	struct Interpolators
	{
		Vector4 Color;
		Vector2 TexCoords;
	};

	using VertexShader = std::function<Interpolators(const VertexShaderInput&)>;
	using FragmentShader = std::function<Vector4(const Interpolators&)>;


#define BIND_VERTEX_SHADER(shader) [this](const VertexShaderInput& input) {return shader.VertexShader(input); }
#define BIND_FRAGMENT_SHADER(shader) [this](const Interpolators& interpolators) {return shader.FragmentShader(interpolators); }


	class Shader
	{
	public:
		friend class Rasterizer;

	protected:
		Ref<Texture> GetTexture(uint8 slot) { return m_TextureSlots->at(slot); }

	protected:
		Vector4 Vertex_Position;
		Vector2 Fragment_PixelCoords;

	private:
		std::unordered_map<uint8, Ref<Texture>>* m_TextureSlots = nullptr;
	};
}
