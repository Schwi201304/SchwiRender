#include "swpch.h"
#include "Mesh.h"

namespace schwi {
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
}