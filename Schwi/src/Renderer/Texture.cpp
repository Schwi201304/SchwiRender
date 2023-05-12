#include "swpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace schwi {
	Ref<Texture2D> Texture2D::Create(const std::string& path, TextureType type)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SW_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path, type);
		}

		SW_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}