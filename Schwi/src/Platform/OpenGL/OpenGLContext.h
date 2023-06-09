#pragma once

#include "Renderer/Context.h"

struct GLFWwindow;

namespace schwi {

	class SCHWI_API OpenGLContext : public Context
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}