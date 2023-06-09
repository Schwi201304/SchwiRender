#pragma once
#include "Core/Core.h"

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace schwi {
	class Mesh;
	class Texture2D;	
	enum class TextureType;
	class Shader;

	class SCHWI_API Model
	{
	public:
		Model(std::string const& path,
			bool gamma = false);

		Model(Ref<Mesh> mesh) { m_Meshes.push_back(mesh); }

		void Draw(Ref<Shader>& shader);
		void DrawOutline();

		glm::mat4 GetTransform() { return m_Transform; }
		void SetTransform(const glm::mat4& ts) { m_Transform = ts; }

		void SetDrawOutline(bool draw) { m_DrawOutline = draw; }

	private:
		std::vector<Ref<Texture2D>> m_TexturesLoaded;
		std::vector<Ref<Mesh>>    m_Meshes;
		std::string m_Directory;
		glm::mat4 m_Transform = glm::mat4(1.0f);
		bool m_GammaCorrection;
		bool m_DrawOutline = false;
		Ref<Shader> m_OutlineShader;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		Ref<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType type_);
	};

}