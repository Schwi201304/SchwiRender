#pragma once

#include <entt/entt.hpp>

namespace schwi {

#define MAX_LIGHT_NUM 8
	class Shader;
	class Camera;
	class CameraController;

	class SCHWI_API Scene
	{
	public:
		Scene();
		~Scene() = default;

		void Draw(const Ref<Camera>&);

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static Ref<SceneData>GetSceneData() { return s_SceneData; }
		Ref<CameraController> m_CameraController=CreateRef<CameraController>();
	private:
		void DrawLight();

		Ref<Shader> m_Shader;
		Ref<Shader> m_DefaultShader;
		bool m_EnableLineMode = false;
		static Ref<SceneData> s_SceneData;
		entt::registry m_Registry;

		friend class EditorLayer;
		friend class SceneLayer;
	};

}