 #pragma once

#include <string>
#include <map>
#include <glm/vec2.hpp>

typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace RenderEngine 
{
	class Texture2D
	{
	public:
		struct SubTexture2D 
		{
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;

			SubTexture2D(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV) 
				: leftBottomUV(_leftBottomUV)
				, rightTopUV(_rightTopUV)
			{	}
			SubTexture2D()
				: leftBottomUV(0.0f)
				, rightTopUV(1.0f)
			{	}
		};

		Texture2D(const GLuint width, const GLuint height, unsigned char* data, const unsigned int channels = 4);
		~Texture2D();

		Texture2D() = delete;
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D& operator=(Texture2D&& texture2D) noexcept;
		Texture2D(Texture2D&& texture2D) noexcept;

    void updateData(const GLuint width, const GLuint height, unsigned char* data, const unsigned int channels);

		void bind() const;
		unsigned int width() { return m_width; };
		unsigned int height() { return m_height; };
		void addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
		const SubTexture2D& getSubTexture(std::string name);
	private:
		GLuint m_ID;
		GLenum m_mode;
		unsigned int m_width;
		unsigned int m_height;

		std::map<std::string, SubTexture2D> m_subTextures;
	};
}