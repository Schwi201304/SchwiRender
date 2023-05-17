#pragma once


#include "Core/Core.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Model.h"
#include "Renderer/Light.h"
#include "CameraController.h"

namespace schwi {

#define MAX_LIGHT_NUM 8

	class SCHWI_API Scene
	{
	public:
		Scene();
		~Scene() = default;

		void Draw();

		Ref<Model> GetModel(uint32_t i) { return m_ModelList[i]; }

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static Ref<SceneData>GetSceneData() { return s_SceneData; }
		Ref<CameraController> m_CameraController;
	private:

		std::vector<Ref<Shader>> m_ShaderList;
		std::vector<Ref<Model>> m_ModelList;
		std::vector<Ref<PointLight>> m_PointLightList;
		Ref<Shader> m_Shader, m_DefaultShader;
		Ref<Mesh> sphere,plane;

		bool m_EnableLineMode = false;
		static Ref<SceneData> s_SceneData;

		friend class EditorLayer;
		friend class SceneLayer;
	};

}