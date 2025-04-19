#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/ShaderProgramLayout.h"
#include "EngineCore/System/Log.h"
#include <glm/gtc/type_ptr.hpp>

namespace RenderEngine 
{
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader, RenderEngine::ShaderProgramLayout* layout)
		: m_layout(std::move(layout))
	{
		GLuint vertexShaderID;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
		{
			LOG_ERROR("[shader-program]: VERTEX SHADER ERROR COMPILE");
			return;
		}
		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
		{
			LOG_ERROR("[shader-program]: FRAGMENT SHADER ERROR COMPILE");
			glDeleteShader(vertexShaderID);
			return;
		}
		GLuint geometryShaderID;
		if (!geometryShader.empty())
		{
			if (!createShader(geometryShader, GL_GEOMETRY_SHADER, geometryShaderID))
			{
				LOG_ERROR("[shader-program]: GEOMETRY SHADER ERROR COMPILE");
				glDeleteShader(vertexShaderID);
				glDeleteShader(fragmentShaderID);
				return;
			}
		}
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShaderID);
		glAttachShader(m_ID, fragmentShaderID);
		if (!geometryShader.empty()) glAttachShader(m_ID, geometryShaderID);
		glLinkProgram(m_ID);

		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[1024];
			glGetProgramInfoLog(m_ID, 1024, nullptr, infoLog);
			LOG_ERROR("[shader-program]: Link: {0}", infoLog);
		}
		else
		{
			m_isCompiled = true;
		}
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		if (!geometryShader.empty()) glDeleteShader(geometryShaderID);
	}
	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_ID);
		m_cacheUniformMap.clear();
	}
	bool ShaderProgram::createShader(const std::string& sousce, const GLenum shaderType, GLuint& shaderID) 
	{
		shaderID = glCreateShader(shaderType);
		const char* code = sousce.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
			LOG_ERROR("[shader-program]: Compile: {0}", infoLog);
			return false;
		}
		return true;
	}
	bool ShaderProgram::getLocation(const std::string& name, GLuint& location) const
	{
		cacheMap::const_iterator it = m_cacheUniformMap.find(name);
		if (it != m_cacheUniformMap.end())
		{
			location = it->second;
			return true;
		}
		return false;
	}
	void ShaderProgram::addLocation(const std::string& name, GLuint& location)
	{
		m_cacheUniformMap.emplace(name, location);
	}
	void ShaderProgram::use() const {
		glUseProgram(m_ID);
	}
	void ShaderProgram::setInt(const std::string& name, const GLint& value)
	{
		GLuint location;
		if (getLocation(name, location))
		{
			glUniform1i(location, value);
			return;
		}
		location = glGetUniformLocation(m_ID, name.c_str());
		addLocation(name, location);
		glUniform1i(location, value);
	}
	void ShaderProgram::setFloat(const std::string& name, const GLfloat& value) 
	{
		GLuint location;
		if (getLocation(name, location))
		{
			glUniform1f(location, value);
			return;
		}
		location = glGetUniformLocation(m_ID, name.c_str());
		addLocation(name, location);
		glUniform1f(location, value);
	}
	void ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& matrix)
	{
		GLuint location;
		if (getLocation(name, location))
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
			return;
		}
		location = glGetUniformLocation(m_ID, name.c_str());
		addLocation(name, location);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void ShaderProgram::setVec4(const std::string& name, const glm::vec4& vec)
	{
		GLuint location;
		if (getLocation(name, location))
		{
			glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
			return;
		}
		location = glGetUniformLocation(m_ID, name.c_str());
		addLocation(name, location);
		glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}
	void ShaderProgram::setVec3(const std::string& name, const glm::vec3& vec)
	{
		GLuint location;
		if (getLocation(name, location))
		{
			glUniform3f(location, vec.x, vec.y, vec.z);
			return;
		}
		location = glGetUniformLocation(m_ID, name.c_str());
		addLocation(name, location);
		glUniform3f(location, vec.x, vec.y, vec.z);
	}
	void ShaderProgram::setVec2(const std::string& name, const glm::vec2& vec)
	{
		GLuint location;
		if (getLocation(name, location))
		{
			glUniform2f(location, vec.x, vec.y);
			return;
		}
		location = glGetUniformLocation(m_ID, name.c_str());
		addLocation(name, location);
		glUniform2f(location, vec.x, vec.y);
	}
	RenderEngine::ShaderProgramLayout* ShaderProgram::get_layout()
	{
		return m_layout;
	}
	bool ShaderProgram::isCompiled() const {
		return m_isCompiled;
	}
	ShaderProgram& ShaderProgram::operator=(ShaderProgram && shaderProgram) noexcept
	{
		glDeleteProgram(m_ID);
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
		return *this;
	}
	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
	}

}
