#include "swpch.h"
#include "SceneLayer.h"

#include <ImGuizmo.h>

#include "Core/Util.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Buffer.h"
#include "Components.h"
#include "Entity.h"
#include "CameraController.h"
#include "SceneSerializer.h"

namespace schwi {
	Ref<SceneLayer> SceneLayer::s_Instance = nullptr;

	Entity SceneLayer::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Scene->m_Registry.create(), s_Instance };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void SceneLayer::DestroyEntity(Entity entity)
	{
		m_Scene->m_Registry.destroy(entity);
	}


	SceneLayer::SceneLayer()
		:Layer("Scene Layer")
	{
		s_Instance.reset(this);
		m_ViewportSize = { 800.0f,600.0f };
		m_Scene = CreateRef<Scene>();

	}

	void SceneLayer::OnAttach()
	{
		RenderCommand::Init();
		m_FrameBuffer = FrameBuffer::Create(m_ViewportSize.x, m_ViewportSize.y);
	}

	void SceneLayer::OnUpdate(Timestep ts)
	{
		Ref<Camera> mainCamera = nullptr;
		{
			auto group = m_Scene->m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = camera.camera;
					mainCamera->SetPosition(transform.Translation);
					mainCamera->SetPitch(transform.Rotation.x);
					mainCamera->SetYaw(transform.Rotation.y);
					break;
				}
			}
		}
		{
			auto group = m_Scene->m_Registry.view<TransformComponent, LightComponent>();
			for (auto entity : group)
			{
				auto [transform, light] = group.get< TransformComponent, LightComponent>(entity);
				light.light->Position = transform.Translation;
			}
		}

		if (mainCamera)
		{
			BeginScene();
			if (mainCamera != m_Scene->m_CameraController->GetCamera())
				m_Scene->m_CameraController->SetCamera(mainCamera);
			m_Scene->m_CameraController->OnUpdate(ts);
			m_Scene->Draw(mainCamera);
			EndScene();
		}
	}

	void SceneLayer::OnImGuiRender()
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove);

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, m_ViewportSize.x, m_ViewportSize.y);
		auto camera = SceneLayer::GetInstance()->GetScene()->m_CameraController->GetCamera();
		auto view = camera->GetViewMatrix();
		auto proj = camera->GetProjectionMatrix();
		ImGuizmo::DrawGrid(glm::value_ptr(view), glm::value_ptr(proj), glm::value_ptr(identityMatrix), 100.f);
		ImGui::PopStyleColor(1);

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		uint32_t textureID = m_FrameBuffer->GetColorAttachment();
		//m_Scene->m_CameraController->SetActive(ImGui::IsWindowFocused());
		if (m_ViewportSize != *reinterpret_cast<glm::vec2*>(&panelSize))
		{
			m_ViewportSize = { panelSize.x,panelSize.y };
			m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_Scene->m_CameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}
		//ImGui::Text("point:%p\tresolution:%d * %d,%f %f", textureID, m_ScreenWidth, m_ScreenHeight, panelSize.x, panelSize.y);

		ImGui::Image((void*)(intptr_t)textureID,
			ImVec2(m_ViewportSize.x, m_ViewportSize.y),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}

	void SceneLayer::OnEvent(Event& e)
	{
		m_Scene->m_CameraController->OnEvent(e);
	}

	void SceneLayer::NewScene()
	{
		m_Scene = CreateRef<Scene>();
		auto entity = CreateEntity("Camera");
		auto& camera = entity.AddComponent<CameraComponent>();
		camera.camera = CreateRef<Camera>();
		auto& ts = entity.GetComponent<TransformComponent>();
		ts.Translation = camera.camera->GetPosition();
		ts.Rotation = camera.camera->GetRotate();
	}

	void SceneLayer::OpenScene()
	{
		auto path= FileDialogs::OpenFile("Schwi Scene (*.schwi)\0*.schwi\0");
		if (!path.empty())
		{
			m_Scene = CreateRef<Scene>();
			SceneSerializer serializer(s_Instance);
			serializer.Deserialize(path);
		}
	}

	void SceneLayer::SaveScene()
	{
		std::string filepath = FileDialogs::SaveFile("Schwi Scene (*.schwi)\0*.schwi\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(s_Instance);
			serializer.Serialize(filepath);
		}
	}

	void SceneLayer::BeginScene()
	{
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor(m_ClearColor);
		RenderCommand::Clear();
	}

	void SceneLayer::EndScene()
	{
		m_FrameBuffer->Unbind();
	}

	Entity SceneLayer::GetPrimaryCameraEntity()
	{
		auto view = m_Scene->m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, s_Instance };
		}
		return {};
	}

	template<typename T>
	void SceneLayer::OnComponentAdded(Entity entity, T& component)
	{
	}

	template<>
	void SceneLayer::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void SceneLayer::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera = CreateRef<Camera>();
		component.camera->SetAspect(m_ViewportSize.x / m_ViewportSize.y);
	}

	template<>
	void SceneLayer::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void SceneLayer::OnComponentAdded<LightComponent>(Entity entity, LightComponent& component)
	{
		component.light = CreateRef<PointLight>();
		component.lightType = LightType::LightType_PointLight;		
	}

	template<>
	void SceneLayer::OnComponentAdded<ModelComponent>(Entity entity, ModelComponent& component)
	{
	}

	template<>
	void SceneLayer::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
	{
	}
}