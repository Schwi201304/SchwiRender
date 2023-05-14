#pragma once
#include "Renderer/Buffer.h"

namespace schwi {
	class SCHWI_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class SCHWI_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class SCHWI_API OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const uint32_t& width, const uint32_t& height);
		virtual uint32_t GetColorAttachment() const override { return m_ColorAttachment; }
		virtual uint32_t GetDepthAttachment() const override { return m_DepthAttachment; }
		virtual void Bind() const override;
		virtual void Unbind() const override;
		void Resize(uint32_t width, uint32_t height);
	private:
		void Configuration();

		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		uint32_t m_Width, m_Height;
	};
}