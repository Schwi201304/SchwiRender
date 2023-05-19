#include "swpch.h"
#include "Hierarchy.h"

#include "Scene/Components.h"
#include "Core/Util.h"

namespace schwi {
	Hierarchy::Hierarchy(const Ref<SceneLayer>& context)
	{
	}

	void Hierarchy::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		SceneLayer::GetInstance()->GetRegistry().each([&](auto entityID)
			{
				Entity entity{ entityID , SceneLayer::GetInstance() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		if (ImGui::BeginPopupContextWindow("Hierarchy Menu"))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				SceneLayer::GetInstance()->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}


		ImGui::End();

		ImGui::Begin("Inspector");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Light"))
				{
					m_SelectionContext.AddComponent<LightComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Model"))
				{
					m_SelectionContext.AddComponent<ModelComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Mesh"))
				{
					m_SelectionContext.AddComponent<MeshComponent>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}



		ImGui::End();
	}

	void Hierarchy::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if ((ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)) && ImGui::IsItemHovered())
		{
			m_SelectionContext = entity;
		}
		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			//	bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			//	if (opened)
			//		ImGui::TreePop();
			ImGui::TreePop();
		}
		if (m_SelectionContext == entity)
		{
			bool entityDeleted = false;
			if (ImGui::BeginPopupContextItem("Hierarchy Menu"))
			{
				if (ImGui::MenuItem("Delete Entity"))
					entityDeleted = true;

				ImGui::EndPopup();
			}
			if (entityDeleted)
			{
				SceneLayer::GetInstance()->DestroyEntity(entity);
				m_SelectionContext = {};
			}

		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float speed = 1.0f, float resetValue = 0.0f, float columnWidth = 90.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
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

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
			if (open)
			{
				auto& ts = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", ts.Translation, 0.1f);
				DrawVec3Control("Rotation", ts.Rotation, 1.0f);
				DrawVec3Control("Scale", ts.Scale, 0.1f, 1.0f);

				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera"))
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
					float verticalFov = camera->GetFov();
					if (ImGui::DragFloat("FOV", &verticalFov))
						camera->SetFov(verticalFov);

				}

				if (camera->GetCameraType() == CameraType::Orthographic)
				{
					float orthoHeight = camera->GetHeight();
					if (ImGui::DragFloat("Height", &orthoHeight))
						camera->SetHeight(orthoHeight);
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
		if (entity.HasComponent<LightComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(LightComponent).hash_code(), treeNodeFlags, "Light");
			if (open)
			{
				auto& lc = entity.GetComponent<LightComponent>();
				auto light = lc.light;
				ImGui::ColorEdit3("Color", glm::value_ptr(light->Color));
				ImGui::SliderFloat("Intensity", &light->Intensity, 0.001, 10.0);

				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<ModelComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(ModelComponent).hash_code(), treeNodeFlags, "Model");
			if (open)
			{
				auto& mc = entity.GetComponent<ModelComponent>();
				if (ImGui::Button("Open New Model"))
				{
					auto path = FileDialogs::OpenFile("Model (*.obj)\0*.obj\0");
					if (!path.empty())
					{
						mc.path = path;
						mc.model = CreateRef<Model>(path);
					}
				}
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<MeshComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(MeshComponent).hash_code(), treeNodeFlags, "Model");
			if (open)
			{
				auto& mc = entity.GetComponent<MeshComponent>();
				const char* meshTypeStrings[] = { "None","Plane","Cube","Sphere","TriangleMesh"};
				const char* currentMeshTypeString = meshTypeStrings[(int)mc.type];
				if (ImGui::BeginCombo("MeshType", currentMeshTypeString))
				{
					for (int i = 0; i < 5; i++)
					{
						bool isSelected = currentMeshTypeString == meshTypeStrings[i];
						if (ImGui::Selectable(meshTypeStrings[i], isSelected))
						{
							currentMeshTypeString = meshTypeStrings[i];
							mc.type = (MeshType)i;
							mc.mesh = CreateRef<Mesh>(mc.type, mc.sample);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				if (ImGui::InputInt("Sample", &mc.sample))
				{
					mc.mesh = CreateRef<Mesh>(mc.type, mc.sample);
				}
				ImGui::TreePop();
			}
		}
	}

}