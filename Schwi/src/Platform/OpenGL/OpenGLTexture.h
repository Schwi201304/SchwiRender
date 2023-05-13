#pragma once
#include "Renderer/Texture.h"

namespace schwi {
	class SCHWI_API OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path, TextureType type);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual TextureType GetType()const override { return m_TextureType; }
		virtual std::string GetPath() const override { return m_Path; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		TextureType m_TextureType;
		std::string m_Path;
		uint32_t m_Width, m_Height, m_Channel;
		uint32_t m_TextureID;
	};
}