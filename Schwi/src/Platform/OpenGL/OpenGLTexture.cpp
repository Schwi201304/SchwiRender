#include "swpch.h"
#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

namespace schwi {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channel;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channel, 0);
		SW_TRACE("Loading image:'{0}' {1}*{2} channels:{3}", path, width, height, channel);
		SW_ASSERT(data, "Failed to load image!");
		GLenum internalFormat = 0, dataFormat = 0;
		if (channel == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channel == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		SW_ASSERT(internalFormat & dataFormat, "Format not supported!");
		//SW_DEBUG("{:0x} {:0x}", internalFormat, dataFormat);
		m_Width = width;
		m_Height = height;
		m_Channel = channel;

		glCreateTextures(GL_TEXTURE_2D, 1,&m_TextureID);
		glTextureStorage2D(m_TextureID, 4, internalFormat, m_Width, m_Height);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(m_TextureID);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}
}