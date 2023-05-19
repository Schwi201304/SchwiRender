#include "swpch.h"
#include "EditorLayer.h"

namespace schwi {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
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