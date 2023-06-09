#pragma once

#include "Core/Core.h"
#include "Texture.h"

namespace schwi {
	class VertexArray;
	struct PhongMaterial;
	class Shader;
	class Texture2D;

	struct SCHWI_API Vertex {
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	enum class SCHWI_API MeshType {
		None,
		Plane,
		Cube,
		Sphere,
		TriangleMesh
	};

	class SCHWI_API Mesh {
	public:
		std::vector<Vertex>              m_Vertices;
		std::vector<unsigned int>        m_Indices;
		Ref<VertexArray> m_VertexArray;
		Ref<PhongMaterial> m_Material;

		bool m_DrawOutline = false;

		Mesh(std::vector<Vertex> vertices,
			std::vector<uint32_t> indices,
			std::unordered_map<TextureType, Ref<Texture2D>> textures);
		Mesh(MeshType meshType, uint32_t sample);

		void Draw(Ref<Shader>& shader, const glm::mat4& transform);

	private:
		void GenVertexArray();
		void CreatePlane(const uint32_t& sample);
		void CreateCube(const uint32_t& sample);
		void CreateSphere(const uint32_t& sample);
	};
}