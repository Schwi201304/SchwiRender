#include "swpch.h"
#include "Application.h"

#include "Renderer/Renderer.h"
#include "Core/Input.h"
#include "Core/Core.h"

#include <glfw/glfw3.h>

namespace schwi {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SW_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(SW_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		m_ImGuiLayer = CreateRef<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer);
		
		m_ScreenShader = Shader::Create(std::string(SOLUTION_DIR) + "assets/shaders/screen.glsl");
		auto [w, h] = Application::Get().GetWindow().GetResolution();
		m_FrameBuffer = FrameBuffer::Create(w, h);
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
		const ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse||io.WantCaptureKeyboard) {
			return;
		}
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
			m_FrameBuffer->Bind();

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

			m_FrameBuffer->Unbind();

			RenderCommand::SetClearColor(m_ClearColor);
			RenderCommand::Clear();
			m_ImGuiLayer->Begin();
			for (auto layer : m_LayerStack)
				layer->OnImGuiRender();
			ImGui::Begin("Scene");
			uint32_t textureID = m_FrameBuffer->GetColorAttachment();
			auto [w, h] = Application::Get().GetWindow().GetResolution();
			ImGui::Text("point:%p", textureID);
			ImGui::SameLine();
			ImGui::Text("resolution:%d * %d", w, h);
			ImGui::Image((void*)(intptr_t)textureID, ImVec2(w, h), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
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
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}