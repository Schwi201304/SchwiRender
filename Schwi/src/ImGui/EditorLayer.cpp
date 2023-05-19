#include "swpch.h"
#include "EditorLayer.h"

namespace schwi {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		m_SceneLayer = SceneLayer::GetInstance();

		m_Hierarchy.SetContext(m_SceneLayer);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{

	}

	void EditorLayer::OnImGuiRender()
	{
		m_Hierarchy.OnImGuiRender();
	}

	void EditorLayer::OnEvent(Event& e)
	{

	}
}