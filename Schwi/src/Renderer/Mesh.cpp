#include "swpch.h"
#include "Mesh.h"

namespace schwi {
	Mesh::Mesh(std::vector<Vertex> vertices,
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

	Mesh::Mesh(MeshType meshType, uint32_t sample)
	{
		m_Material = CreateRef<PhongMaterial>();
		switch (meshType)
		{
		case MeshType::Plane:
			CreatePlane(sample);
			break;
		case MeshType::Cube:
			CreateCube(sample);
			break;
		case MeshType::Sphere:
			CreateSphere(sample);
			break;
		default:
			SW_ERROR("Unsupported MeshType!");
		}
		GenVertexArray();
	}

	void Mesh::Draw(Ref<Shader>& shader, const glm::mat4& transform)
	{
		m_Material->m_Shader = shader;
		m_Material->Bind();
		m_VertexArray->Bind();
		shader->SetMat4("u_Transform", transform);
		RenderCommand::DrawIndexed(m_VertexArray);
	}

	void Mesh::CreatePlane(const uint32_t& sample)
	{
		for (uint32_t i = 0; i <= sample; i++)
			for (uint32_t j = 0; j <= sample; j++)
			{
				Vertex v;
				v.Position = { (float)i / (float)sample - 0.5f, 0.0f, (float)j / (float)sample - 0.5f };
				v.Normal = { 0.0f, 1.0f, 0.0f };
				v.TexCoords = { (float)i / (float)sample, (float)j / (float)sample };
				m_Vertices.push_back(v);
			}
		uint32_t indices[6] = { 0, sample + 1, sample + 2, 0, sample + 2,1 };
		for (int k = 0; k < (sample + 1) * sample; k++)
			for (int i = 0; i < 6; i++)
				if ((k + 1) % (sample + 1) > 0)
					m_Indices.push_back(indices[i] + k);
	}

#define DOUBLE_FOR(code)																\
for (uint32_t i = 0; i <= sample; i++)													\
	for (uint32_t j = 0; j <= sample; j++)												\
	{																					\
		Vertex v;																		\
		v.TexCoords =glm::vec2((float)i / (float)sample, (float)j / (float)sample );	\
		code																			\
		m_Vertices.push_back(v);														\
	}

	void Mesh::CreateCube(const uint32_t& sample)
	{
		//	back face
		DOUBLE_FOR({
			v.Position = glm::vec3((float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f, -0.5f);
			v.Normal = glm::vec3(0.0f, 0.0f, -1.0f);
			});
		//	front face
		DOUBLE_FOR({
			v.Position = glm::vec3((float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f, 0.5f);
			v.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
			});
		//	left face
		DOUBLE_FOR({
			v.Position = glm::vec3(-0.5f, (float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f);
			v.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
			});
		//	right face
		DOUBLE_FOR({
			v.Position = glm::vec3(0.5f, (float)i / (float)sample - 0.5f, (float)j / (float)sample - 0.5f);
			v.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
			});
		//	bottom face
		DOUBLE_FOR({
			v.Position = glm::vec3((float)i / (float)sample - 0.5f, -0.5f, (float)j / (float)sample - 0.5f);
			v.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
			});
		// top face
		DOUBLE_FOR({
			v.Position = glm::vec3((float)i / (float)sample - 0.5f, 0.5f, (float)j / (float)sample - 0.5f);
			v.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			});

		uint32_t indices_1[6] = { 0, sample + 2, sample + 1, sample + 2, 0, 1 };
		uint32_t indices_2[6] = { 0, sample + 2, sample + 1, sample + 2, 1, 0 };
		for (uint32_t k = 0; k < 6; k++)
			for (uint32_t j = 0; j < (sample + 1) * sample; j++)
				for (uint32_t i = 0; i < 6; i++)
					if ((j + 1) % (sample + 1) > 0)
						if (k % 2)
							m_Indices.push_back(indices_1[i] + j + k * (sample + 1) * (sample + 1));
						else
							m_Indices.push_back(indices_2[i] + j + k * (sample + 1) * (sample + 1));
	}

	void Mesh::CreateSphere(const uint32_t& sample)
	{
		uint32_t real = sample * 10;
		for (uint32_t i = 0; i <= real; i++)
			for (uint32_t j = 0; j <= real; j++)
			{
				Vertex v;
				v.Position = { (float)i / (float)real, 0.0f, (float)j / (float)real };
				v.Normal = { 0.0f, 1.0f, 0.0f };
				v.TexCoords = { (float)i / (float)real, (float)j / (float)real };
				m_Vertices.push_back(v);
			}
		uint32_t indices[6] = { 0, real + 1, real + 2, 0, real + 2,1 };
		for (uint32_t k = 0; k < (real + 1) * real; k++)
			for (uint32_t i = 0; i < 6; i++)
				if ((k + 1) % (real + 1) > 0)
					m_Indices.push_back(indices[i] + k);
		for (auto& p : m_Vertices)
		{
			float phi = glm::radians(360.0f * p.Position.z);
			float theta = glm::radians(180.0f * p.Position.x - 90.0f);
			p.Position.x = p.Normal.x = cos(theta) * cos(phi);
			p.Position.y = p.Normal.y = sin(theta);
			p.Position.z = p.Normal.z = cos(theta) * sin(phi);
		}
	}

	void Mesh::GenVertexArray()
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
}