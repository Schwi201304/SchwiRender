#include "swpch.h"
#include "Hierarchy.h"

#include "imgui.h"
#include "Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>

namespace schwi {
	Hierarchy::Hierarchy(const Ref<SceneLayer>& context)
	{
		SetContext(context);
	}

	void Hierarchy::SetContext(const Ref<SceneLayer>& context)
	{
		m_Context = context;
	}

	void Hierarchy::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID , m_Context };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		ImGui::End();

		ImGui::Begin("Inspector");
		if (m_SelectionContext)
			DrawComponents(m_SelectionContext);

		ImGui::End();
	}

	void Hierarchy::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

	}

	void Hierarchy::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[32];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& translation = entity.GetComponent<TransformComponent>().Translation;
				ImGui::DragFloat3("Tr", glm::value_ptr(translation), 0.1f);
				auto& rotation = entity.GetComponent<TransformComponent>().Rotation;
				ImGui::DragFloat3("Rt", glm::value_ptr(rotation), 1.0f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeStrings[] = { "Orthographic","Perspective" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera->GetCameraType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera->SetCameraType((CameraType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera->GetCameraType() == CameraType::Perspective)
				{
					float verticalFov =camera->GetFov();
					if (ImGui::DragFloat("FOV", &verticalFov))
						camera->SetFov(verticalFov);

				}

				if (camera->GetCameraType() == CameraType::Orthographic)
				{
					float orthoHeight = camera->GetHeight();
					if (ImGui::DragFloat("Height", &orthoHeight))
						camera->SetHeight(orthoHeight);

					//float orthoNear = camera->GetNearPlane();
					//if (ImGui::DragFloat("Near", &orthoNear))
					//	camera->SetNearPlane(orthoNear);
					//
					//float orthoFar = camera->GetFarPlane();
					//if (ImGui::DragFloat("Far", &orthoFar))
					//	camera->SetFarPlane(orthoFar);

					//ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
				}

				float perspNear = camera->GetNearPlane();
				if (ImGui::DragFloat("Near", &perspNear))
					camera->SetNearPlane(perspNear);

				float perspFar = camera->GetFarPlane();
				if (ImGui::DragFloat("Far", &perspFar))
					camera->SetFarPlane(perspFar);


				ImGui::TreePop();
			}
		}
	}
}