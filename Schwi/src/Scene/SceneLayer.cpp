#include "swpch.h"
#include "SceneLayer.h"
#include "Renderer/RenderCommand.h"
#include "imgui.h"

namespace schwi {
	Ref<SceneLayer> SceneLayer::s_Instance = nullptr;

	SceneLayer::SceneLayer()
		:Layer("Scene Layer")
	{
		s_Instance.reset(this);
		m_Scene = CreateRef<Scene>();

	}

	void SceneLayer::OnAttach()
	{
		RenderCommand::Init();
		m_ViewportSize = { 800.0f,600.0f };
		m_FrameBuffer = FrameBuffer::Create(m_ViewportSize.x, m_ViewportSize.y);
	}

	void SceneLayer::OnUpdate(Timestep ts)
	{
		BeginScene();
		m_Scene->m_CameraController->OnUpdate(ts);
		m_Scene->Draw();
		EndScene();
	}

	void SceneLayer::OnImGuiRender()
	{
		ImGui::Begin("Scene",nullptr,ImGuiWindowFlags_NoMove);
		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		uint32_t textureID = m_FrameBuffer->GetColorAttachment();
		m_Scene->m_CameraController->SetActive(ImGui::IsWindowFocused());
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
}