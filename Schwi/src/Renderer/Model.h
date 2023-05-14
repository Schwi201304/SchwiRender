#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

#include "Core/Core.h"
#include "Mesh.h"


namespace schwi {
	class SCHWI_API Model
	{
	public:
		std::vector<Ref<Texture2D>> m_TexturesLoaded;
		std::vector<Ref<Mesh>>    m_Meshes;
		std::string m_Directory;
		glm::mat4 m_Transform;
		bool m_GammaCorrection;
		bool m_DrawOutline = false;
		Ref<Shader> m_OutlineShader = Shader::Create(std::string(SOLUTION_DIR) + "assets/shaders/default.glsl");

		Model(std::string const& path,
			const glm::mat4& transform = glm::mat4(1.0f),
			bool gamma = false);

		void Draw(Ref<Shader>& shader);
		void DrawOutline();

	private:
		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		Ref<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType type_);
	};

}