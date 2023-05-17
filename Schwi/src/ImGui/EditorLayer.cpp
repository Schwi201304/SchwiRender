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
		m_Entity = m_SceneLayer->CreateEntity();
		m_Entity.AddComponent<TransformComponent>();

		m_CameraEntity = m_SceneLayer->CreateEntity("Camera Entity");
		auto camera = m_SceneLayer->GetInstance()->GetScene()->m_CameraController->GetCamera();
		m_CameraEntity.AddComponent<CameraComponent>(camera);
		TransformComponent tc{ camera->GetPosition(),camera->GetRotate(),glm::vec3(1.0f) };
		m_CameraEntity.AddComponent<TransformComponent>(tc);

	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{

	}

	void EditorLayer::OnImGuiRender()
	{
		if (ImGui::Begin("Camera"))
		{
			ImGui::DragFloat3("Tr",
				glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Translation));
			ImGui::DragFloat3("Rt",
				glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Rotation));
			ImGui::End();
		}

	}

	void EditorLayer::OnEvent(Event& e)
	{

	}
}