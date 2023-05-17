#pragma once

#include <entt/entt.hpp>
#include "Layer/Layer.h"
#include "Renderer/Buffer.h"
#include "Scene.h"


namespace schwi {

	class Entity;

	class SCHWI_API SceneLayer : public Layer
	{
	public:
		SceneLayer();
		~SceneLayer() = default;

		Entity CreateEntity(const std::string& name = std::string());
		entt::registry& Reg() { return m_Registry; }

		virtual void OnAttach() override;
		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		inline float GetAspect() { return m_ViewportSize.x / m_ViewportSize.y; }
		inline glm::vec2 GetViewportSize() { return m_ViewportSize; }
		inline Ref<Scene> GetScene() { return m_Scene; }
		void BeginScene();
		void EndScene();

		static Ref<SceneLayer> GetInstance() { return s_Instance; };

	private:
		static Ref<SceneLayer> s_Instance;
		Ref<Scene> m_Scene;
		entt::registry m_Registry;
		friend class Entity;

		glm::mat4 identityMatrix = glm::mat4(1.0f);
		glm::vec2 m_ViewportSize;
		glm::vec4 m_ClearColor{ 102.f / 255.f, 204.f / 255.f, 1.f, 0.f };
		Ref<FrameBuffer> m_FrameBuffer;
	};

}