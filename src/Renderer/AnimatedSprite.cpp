#include "Sprite.h";

#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer
{
	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, const std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2& position, const glm::vec2& size, const float rotation)
		: m_position(position), m_rotation(rotation), m_size(size), m_pTexture(pTexture), m_pShaderProgram(pShaderProgram)
	{
	
		const GLfloat vertexCoords[] = {
			//2--3  1
			//| / /	|
			//1	 3--2			

			//X --- Y
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,

			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f
		};

		auto subTexture = pTexture->getSubTexture(initialSubTexture);

		const GLfloat textureCoords[] = {				
				//U --- V
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
				subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
				subTexture.rightTopUV.x, subTexture.rightTopUV.y,

				subTexture.rightTopUV.x, subTexture.rightTopUV.y,
				subTexture.rightTopUV.x, subTexture.leftBottomUV.y,
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y
			/*0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,

			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,*/

		}; 

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_vertexCoords);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), &vertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glGenBuffers(1, &m_textureCoords);
		glBindBuffer(GL_ARRAY_BUFFER, m_textureCoords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), &textureCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Sprite::~Sprite()
	{
		glDeleteBuffers(1, &m_vertexCoords);
		glDeleteBuffers(1, &m_textureCoords);
		glDeleteVertexArrays(1, &m_VAO);
	}
	void Sprite::render() const
	{
		m_pShaderProgram->use();

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(m_position, 0.0f)); 
		model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.0f));
		model = glm::rotate(model, glm::radians(m_rotation),  glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.0f));
		model = glm::scale(model, glm::vec3(m_size, 1.0f));

		glBindVertexArray(m_VAO);
		m_pShaderProgram->setMatrix4("modelMat", model);

		glActiveTexture(GL_TEXTURE0);
		m_pTexture->bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	void Sprite::setPosition(glm::vec2& position)
	{
		m_position = position;
	}
	void Sprite::setSize(glm::vec2& size)
	{ 
		m_size = size;
	}
	void Sprite::setRotation(const float rotation)
	{
		m_rotation = rotation;
	}
}