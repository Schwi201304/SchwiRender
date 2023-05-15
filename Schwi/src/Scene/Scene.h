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

		Ref<CameraController> m_CameraController;
	private:
		Ref<Shader> m_Shader, m_DefaultShader;
		Ref<VertexArray> m_LightVao;
		Ref<Model> m_Model;
		Ref<Mesh> m_LightMesh, m_PlaneMesh;
		PointLight m_Light;
		SpotLight m_SpotLight;

		bool m_EnableLineMode = false;

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static Ref<SceneData> s_SceneData;
	};

}