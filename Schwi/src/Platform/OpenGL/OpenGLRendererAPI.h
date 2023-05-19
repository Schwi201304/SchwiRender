#pragma once

#include "Renderer/RendererAPI.h"

namespace schwi {
	class SCHWI_API OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetLineMode(const bool enable) override;
		virtual void SetDepthTest(const bool enable) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void SetStencilMask(const uint32_t& mask) override;
		virtual void SetStencilFunc(uint32_t func, int32_t ref, uint32_t mask) override;
		virtual void SetStencilOp(uint32_t fail, int32_t zfail, uint32_t zpass) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};

}