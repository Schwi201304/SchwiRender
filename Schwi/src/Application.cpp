#include "swpch.h"

#include "Application.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"

#include <GLFW/glfw3.h>

namespace schwi {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(112.0f/255.0f, 204.0f/255.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}