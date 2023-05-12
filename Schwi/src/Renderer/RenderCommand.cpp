#include "swpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace schwi {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}