#include "EngineCore/Renderer/Texture2D.h"

#include <cmath>

namespace RenderEngine 
{
	Texture2D::Texture2D(const GLuint width, const GLuint height, unsigned char* data, const unsigned int channels,
		const GLenum filter, const GLenum wrapMode)
		: m_width(width)
		, m_height(height)
	{
		switch (channels)
		{
		case 3:
			m_mode = GL_RGB;
			break;
		case 4:
			m_mode = GL_RGBA;
			break;
		default:
			m_mode = GL_RGB;
			break;
		}

		const GLsizei mip_levels = static_cast<GLsizei>(log2(std::max(width, height))) + 1;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTextureStorage2D(GL_TEXTURE_2D, mip_levels, m_mode, width, height);
		glTexImage2D(GL_TEXTURE_2D, 0, m_mode, width, height, 0, m_mode, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		glGenerateTextureMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	Texture2D& Texture2D::operator=(Texture2D&& texture2D) noexcept
	{
		glDeleteTextures(1, &m_ID);

		m_ID = texture2D.m_ID;
		texture2D.m_ID = 0;

		m_mode = texture2D.m_mode;

		m_width = texture2D.m_width;
		m_height = texture2D.m_height;
		return *this;
	}
	Texture2D::Texture2D(Texture2D&& texture2D) noexcept
	{
		m_ID = texture2D.m_ID;
		texture2D.m_ID = 0;

		m_mode = texture2D.m_mode;

		m_width = texture2D.m_width;
		m_height = texture2D.m_height;
	}
	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_ID);
	}
	void Texture2D::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
	void Texture2D::addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV)
	{
		m_subTextures.emplace(std::move(name), SubTexture2D(leftBottomUV, rightTopUV));
	}
	const Texture2D::SubTexture2D& Texture2D::getSubTexture(std::string name)
	{
		auto it = m_subTextures.find(name);
		if (it != m_subTextures.end()) 
		{
			return it->second;
		}
		const static SubTexture2D defaultSubTexture2D;
		return defaultSubTexture2D;
	}
}