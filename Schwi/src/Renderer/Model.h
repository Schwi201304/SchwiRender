#pragma once
#include "swpch.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"


namespace schwi {
	class Model
	{
	public:
		// model data 
		// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
		std::vector<Ref<Texture2D>> m_TexturesLoaded;
		std::vector<Mesh>    m_Meshes;
		std::string m_Directory;
		glm::mat4 m_Transform;
		bool m_GammaCorrection;

		// constructor, expects a filepath to a 3D model.
		Model(std::string const& path,
			const glm::mat4& transform = glm::mat4(1.0f),
			bool gamma = false)
			: m_GammaCorrection(gamma), m_Transform(transform)
		{
			loadModel(path);
		}

		void Draw(Ref<Shader>& shader)
		{
			for (unsigned int i = 0; i < m_Meshes.size(); i++)
				m_Meshes[i].Draw(shader, m_Transform);
		}

	private:
		void loadModel(const std::string& path)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
			SW_TRACE("Load model:{}", path);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				SW_ERROR("ASSIMP::{} ", importer.GetErrorString());
				return;
			}
			m_Directory = path.substr(0, path.find_last_of('/'));

			processNode(scene->mRootNode, scene);
		}

		void processNode(aiNode* node, const aiScene* scene)
		{
			for (uint32_t i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				m_Meshes.push_back(processMesh(mesh, scene));
			}
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene);
			}
		}

		Mesh processMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::unordered_map<TextureType, Ref<Texture2D>> textures;

			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				glm::vec3 vector;
				// positions
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.Position = vector;
				// normals
				if (mesh->HasNormals())
				{
					vector.x = mesh->mNormals[i].x;
					vector.y = mesh->mNormals[i].y;
					vector.z = mesh->mNormals[i].z;
					vertex.Normal = vector;
				}
				// texture coordinates
				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					glm::vec2 vec;
					// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
					// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = vec;
					// tangent
					vector.x = mesh->mTangents[i].x;
					vector.y = mesh->mTangents[i].y;
					vector.z = mesh->mTangents[i].z;
					vertex.Tangent = vector;
					// bitangent
					vector.x = mesh->mBitangents[i].x;
					vector.y = mesh->mBitangents[i].y;
					vector.z = mesh->mBitangents[i].z;
					vertex.Bitangent = vector;
				}
				else
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);

				vertices.push_back(vertex);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (uint32_t j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}
			// process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			Ref<Texture2D> diffuseMap = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
			Ref<Texture2D> specularMap = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
			Ref<Texture2D> normalMap = loadMaterialTextures(material, aiTextureType_NORMALS, TextureType::NORMAL);
			Ref<Texture2D> heightMap = loadMaterialTextures(material, aiTextureType_DISPLACEMENT, TextureType::DISPLACEMENT);
			textures.insert({
				{ TextureType::DIFFUSE ,diffuseMap},
				{ TextureType::SPECULAR,specularMap },
				{ TextureType::NORMAL,normalMap},
				{ TextureType::DISPLACEMENT,heightMap} });
			// return a mesh object created from the extracted mesh data
			return Mesh(vertices, indices, textures);
		}

		// checks all material textures of a given type and loads the textures if they're not loaded yet.
		// the required info is returned as a Texture struct.
		Ref<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType type_)
		{
			Ref<Texture2D> texture;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
				bool skip = false;
				for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++)
				{
					if (std::strcmp(m_TexturesLoaded[j]->GetPath().data(), str.C_Str()) == 0)
					{
						texture = m_TexturesLoaded[j];
						skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
						break;
					}
				}
				if (!skip)
				{   // if texture hasn't been loaded already, load it
					std::string path = m_Directory + '/' + std::string(str.C_Str());
					texture = Texture2D::Create(path, type_);
					// store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
					m_TexturesLoaded.push_back(texture);
				}
			}
			return texture;
		}
	};

}