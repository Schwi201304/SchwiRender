#include "swpch.h"
#include "Application.h"

#include "Core/Input.h"
#include "Core/Core.h"
#include "Renderer/RenderCommand.h"
#include "ImGui/ImGuiLayer.h"
#include "ImGui/MenuLayer.h"
#include "ImGui/EditorLayer.h"
#include "Scene/SceneLayer.h"

namespace schwi {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SW_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(SW_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = CreateRef<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer);
		m_SceneLayer = CreateRef<SceneLayer>();
		PushLayer(m_SceneLayer);
		PushLayer(CreateRef<EditorLayer>());
		PushOverlay(CreateRef<MenuLayer>());
	}

	void Application::PushLayer(Ref<Layer> layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Ref<Layer> layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e)
	{
		//const ImGuiIO& io = ImGui::GetIO();
		//if (io.WantCaptureMouse||io.WantCaptureKeyboard) {
		//	return;
		//}
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SW_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SW_BIND_EVENT_FN(Application::OnWindowResize));
		dispatcher.Dispatch<KeyPressedEvent>(SW_BIND_EVENT_FN(Application::OnKeyPressed));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}


	void Application::Run()
	{
		//SW_CORE_DEBUG("{}", m_LayerStack.size());
		while (m_Running)
		{
			RenderCommand::SetClearColor(m_ClearColor);
			RenderCommand::Clear();
			RenderCommand::SetStencilMask(0x00);

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (auto layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			RenderCommand::SetClearColor(m_ClearColor);
			RenderCommand::Clear();
			m_ImGuiLayer->Begin();
			for (auto layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case schwi::Key::Escape:
			m_Running = false;
			return true;
		default:return false;
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		//Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;
	}
}