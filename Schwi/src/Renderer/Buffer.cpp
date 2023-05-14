#include "swpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace schwi {
	Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SW_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		SW_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SW_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		SW_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<FrameBuffer> FrameBuffer::Create(const uint32_t& width, const uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SW_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFrameBuffer>(width, height);
		}

		SW_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}