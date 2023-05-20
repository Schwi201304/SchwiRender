#pragma once

#include "Layer/Layer.h"
#include "Hierarchy.h"

namespace schwi {
	class SceneLayer;

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
		void GizmoEditor();

		static Ref<EditorLayer> s_Instance;
		Hierarchy m_Hierarchy;
		int m_GizmoType = -1;
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		friend class MenuLayer;
		friend class SceneLayer;
	};

}