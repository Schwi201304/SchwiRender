#include "swpch.h"
#include "OpenGLContext.h"

namespace schwi {

#define sw_glGetString(c) reinterpret_cast<const char*>(glGetString(c))

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		SW_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SW_CORE_ASSERT(status, "Failed to initialize Glad!");
		SW_CORE_INFO("OpenGL Info:");
		SW_CORE_INFO("	Vendor: {0}", sw_glGetString(GL_VENDOR));
		SW_CORE_INFO("	Renderer: {0}", sw_glGetString(GL_RENDERER));
		SW_CORE_INFO("	Version: {0}", sw_glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}