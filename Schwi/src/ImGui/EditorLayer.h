#pragma once

#include "Layer/Layer.h"
#include "Scene/SceneLayer.h"
#include "Scene/Entity.h"

namespace schwi {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		static Ref<EditorLayer> GetInstance() { return s_Instance; }
	private:
		static Ref<EditorLayer> s_Instance;
		Ref<SceneLayer> m_SceneLayer;
		Entity m_Entity;
		Entity m_CameraEntity;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};

}