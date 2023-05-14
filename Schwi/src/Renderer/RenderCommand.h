#pragma once

#include "RendererAPI.h"

namespace schwi {
	namespace Stencil {
		enum SCHWI_API : uint32_t
		{
			NEVER = 0x0200,
			LESS, EQUAL, LEQUAL, GREATER, NOTEQUAL, GEQUAL, ALWAYS,
			KEEP = 0x1E00, REPLACE = 0x1E01
		};
	}

	class SCHWI_API RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}
		inline static void SetLineMode(const bool enable)
		{
			s_RendererAPI->SetLineMode(enable);
		}

		inline static void SetDepthTest(const bool enable)
		{
			s_RendererAPI->SetDepthTest(enable);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void SetStencilMask(const uint32_t& mask)
		{
			s_RendererAPI->SetStencilMask(mask);
		}

		inline static void SetStencilFunc(uint32_t func, int32_t ref, uint32_t mask)
		{
			s_RendererAPI->SetStencilFunc(func, ref, mask);
		}

		inline static void SetStencilOp(uint32_t fail, int32_t zfail, uint32_t zpass)
		{
			s_RendererAPI->SetStencilOp(fail, zfail, zpass);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}