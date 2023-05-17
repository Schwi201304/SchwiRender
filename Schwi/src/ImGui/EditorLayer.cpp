#include "swpch.h"
#include "EditorLayer.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Scene/Components.h"
#include "Renderer/Camera.h"

namespace schwi {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		m_SceneLayer = SceneLayer::GetInstance();

		m_Entity = m_SceneLayer->CreateEntity("Light");
		auto light=m_SceneLayer->GetScene()->m_PointLightList[0];
		m_Entity.AddComponent<TransformComponent>(light->Position);
		m_Entity.AddComponent<LightComponent>(light, light->GetLightType());

		m_CameraEntity = m_SceneLayer->CreateEntity("Camera Entity");
		auto camera = m_SceneLayer->GetInstance()->GetScene()->m_CameraController->GetCamera();
		m_CameraEntity.AddComponent<CameraComponent>(camera);
		TransformComponent tc{ camera->GetPosition(),camera->GetRotate(),glm::vec3(1.0f) };
		m_CameraEntity.AddComponent<TransformComponent>(tc);

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