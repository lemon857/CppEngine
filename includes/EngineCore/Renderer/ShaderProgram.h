#pragma once

#include <string>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace RenderEngine
{
	class ShaderProgram {
	public:
		// �������� ��������� ��������� �� ����, ���������� � �����������
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();
		// �������� �� ������������������ ������� 
		bool isCompiled() const;
		// ������������� �������
		void use() const;
		void setInt(const std::string& name, const GLint& value);
		void setFloat(const std::string& name, const GLfloat& value);
		void setMatrix4(const std::string& name, const glm::mat4& matrix);
		void setVec4(const std::string& name, const glm::vec4& vec);

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
	private:
		bool createShader(const std::string& sousce, const GLenum shaderType, GLuint& shaderID);
		bool getLocation(const std::string& name, GLuint& location) const;
		void addLocation(const std::string& name, GLuint& location);
		bool m_isCompiled = false;
		GLuint m_ID = 0;
		typedef std::map<std::string, GLuint> cacheMap;
		cacheMap m_cacheUniformMap;
	};
}