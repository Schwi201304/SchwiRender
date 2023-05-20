#include "swpch.h"
#include "EditorLayer.h"

#include <ImGuizmo.h>
#include "Scene/Components.h"

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
		if (ImGui::IsKeyPressed(ImGuiKey_T))
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_R))
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		if (ImGui::IsKeyPressed(ImGuiKey_F))
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
	}

	void EditorLayer::OnImGuiRender()
	{
		m_Hierarchy.OnImGuiRender();
		GizmoEditor();
	}

	void EditorLayer::GizmoEditor()
	{
		ImGui::Begin("Scene", nullptr);
		Entity& selectedEntity = m_Hierarchy.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();


			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			auto cameraEntity = SceneLayer::GetInstance()->GetPrimaryCameraEntity();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().camera;
			glm::mat4 cameraView = camera->GetViewMatrix();
			const glm::mat4& cameraProjection = camera->GetProjectionMatrix();

			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			bool snap = ImGui::IsKeyPressed(ImGuiKey_LeftCtrl);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);
			ImGuizmo::ViewManipulate(glm::value_ptr(cameraView), glm::length(camera->GetPosition()),
				ImVec2(ImGui::GetWindowPos().x + windowWidth - 128, ImGui::GetWindowPos().y),
				ImVec2(128, 128), 0x10101010);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				ImGuizmo::DecomposeMatrixToComponents(
					glm::value_ptr(transform), glm::value_ptr(translation),
					glm::value_ptr(rotation), glm::value_ptr(scale));

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{

	}
}