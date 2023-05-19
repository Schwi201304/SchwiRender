#pragma once

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Core/Window.h"
#include "Layer/LayerStack.h"

namespace schwi {
	class Shader;
	class ImGuiLayer;
	class SceneLayer;
	class FrameBuffer;

	class SCHWI_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
		void OnEvent(Event& e);
		void PushLayer(Ref<Layer> layer);
		void PushOverlay(Ref<Layer> layer);

		inline Window& GetWindow() { return *m_Window; }
		inline Ref<ImGuiLayer> GetImGuiLayer() { return m_ImGuiLayer; }
		inline bool GetRunStatus() { return m_Running; }
		inline static Application& Get() { return *s_Instance; }

		inline void Close() { m_Running = false; }

	public:
		glm::vec4 m_ClearColor{ 102.f / 255.f, 204.f / 255.f, 1.f, 1.f };

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		Ref<ImGuiLayer> m_ImGuiLayer;
		Ref<SceneLayer> m_SceneLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
		Ref<Shader> m_ScreenShader;
		Ref<FrameBuffer> m_FrameBuffer;
	};

	Application* CreateApplication();
}