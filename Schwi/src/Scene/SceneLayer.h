#pragma once
#include "Layer/Layer.h"
#include "Renderer/Buffer.h"
#include "Scene.h"


namespace schwi {
	class SCHWI_API SceneLayer : public Layer
	{
	public:
		SceneLayer();
		~SceneLayer() = default;

		virtual void OnAttach() override;
		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		inline float GetAspect() { return m_ViewportSize.x/ m_ViewportSize.y; }

		void BeginScene();
		void EndScene();

		static Ref<SceneLayer> GetInstance() { return s_Instance; };

	private:
		static Ref<SceneLayer> s_Instance;
		Ref<Scene> m_Scene;
		glm::vec2 m_ViewportSize;
		glm::vec4 m_ClearColor{ 102.f / 255.f, 204.f / 255.f, 1.f, 1.f };
		Ref<FrameBuffer> m_FrameBuffer;
	};

}