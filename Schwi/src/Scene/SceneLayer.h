#pragma once

#include "Layer/Layer.h"
#include "Scene.h"


namespace schwi {
	class Entity;
	class FrameBuffer;

	class SCHWI_API SceneLayer : public Layer
	{
	public:
		SceneLayer();
		~SceneLayer() = default;

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		virtual void OnAttach() override;
		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		inline float GetAspect() { return m_ViewportSize.x / m_ViewportSize.y; }
		inline glm::vec2 GetViewportSize() { return m_ViewportSize; }
		inline Ref<Scene>& GetScene() { return m_Scene; }
		inline entt::registry& GetRegistry() { return m_Scene->m_Registry; }

		Entity GetPrimaryCameraEntity();
		static Ref<SceneLayer> GetInstance() { return s_Instance; };

		void NewScene();
		void OpenScene();
		void SaveScene();
	private:
		void BeginScene();
		void EndScene();
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		static Ref<SceneLayer> s_Instance;
		Ref<Scene> m_Scene;
		friend class Entity;
		friend class Hierarchy;
		friend class SceneSerializer;

		glm::mat4 identityMatrix = glm::mat4(1.0f);
		glm::vec2 m_ViewportSize;
		glm::vec4 m_ClearColor{ 102.f / 255.f, 204.f / 255.f, 1.f, 0.f };
		Ref<FrameBuffer> m_FrameBuffer;
	};

}