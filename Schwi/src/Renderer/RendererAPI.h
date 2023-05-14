#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace schwi {
	class SCHWI_API RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetLineMode(const bool enable) = 0;
		virtual void SetDepthTest(const bool enable) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetStencilMask(const uint32_t& mask) = 0;
		virtual void SetStencilFunc(uint32_t func, int32_t ref, uint32_t mask) = 0;
		virtual void SetStencilOp(uint32_t fail, int32_t zfail, uint32_t zpass) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}