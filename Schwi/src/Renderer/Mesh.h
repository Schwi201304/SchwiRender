#pragma once
#include "swpch.h"

#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Renderer.h"
#include "Buffer.h"

#define MAX_BONE_INFLUENCE 4

namespace schwi {
	struct Vertex {
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	enum class MeshType {
		None,
		Plane,
		Cube,
		TriMesh
	};

	class Mesh {
	public:
		// mesh Data
		std::vector<Vertex>              m_Vertices;
		std::vector<unsigned int>        m_Indices;
		Ref<VertexArray> m_VertexArray;
		Ref<PhongMaterial> m_Material;
		Ref<Material> m_Mat;

		// constructor
		Mesh(std::vector<Vertex> vertices,
			std::vector<uint32_t> indices,
			std::unordered_map<TextureType, Ref<Texture2D>> textures)
			:m_Vertices(vertices), m_Indices(indices)
		{
			m_Material = CreateRef<PhongMaterial>();
			m_Material->DiffuseTexture = textures[TextureType::DIFFUSE];
			m_Material->SpecularTexture = textures[TextureType::SPECULAR];
			m_Material->NormalTexture = textures[TextureType::NORMAL];
			m_Material->DisplacementTexture = textures[TextureType::DISPLACEMENT];

			GenVertexArray();
		}

		Mesh(uint32_t sample)
		{
			m_Material = CreateRef<PhongMaterial>();
			CreateCube(sample);
			GenVertexArray();
		}

		// render the mesh
		void Draw(Ref<Shader>& shader, const glm::mat4& transform)
		{
			m_Material->m_Shader = shader;
			m_Material->Bind();
			Renderer::Submit(shader, m_VertexArray, transform);
		}

	private:
		void GenVertexArray()
		{
			m_VertexArray = VertexArray::Create();
			Ref<VertexBuffer> vertexBuffer;
			vertexBuffer = VertexBuffer::Create(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
			BufferLayout layout = {
				{ShaderDataType::Float3, "a_Position" },
				{ShaderDataType::Float2, "a_TexCoord" },
				{ShaderDataType::Float3, "a_Normal" },
				{ShaderDataType::Float3, "a_Tangent" },
				{ShaderDataType::Float3, "a_Bitangent" }
			};
			vertexBuffer->SetLayout(layout);
			m_VertexArray->AddVertexBuffer(vertexBuffer);

			Ref<schwi::IndexBuffer> indexBuffer;
			indexBuffer = schwi::IndexBuffer::Create(m_Indices.data(), m_Indices.size());
			m_VertexArray->SetIndexBuffer(indexBuffer);
		}

		void CreateCube(const uint32_t& sample);
	};
}