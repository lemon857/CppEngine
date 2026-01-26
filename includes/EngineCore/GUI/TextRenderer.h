#pragma once 

#include <string>
#include <vector>
#include <glm/mat4x4.hpp>

#include "EngineCore/GUI/GUI_element.h"

namespace RenderEngine
{
	class ShaderProgram;
	class Material;
	class VertexBuffer;
	class VertexArray;
	class IndexBuffer;
}

namespace GUI
{
	class Font;

	class TextRenderer : public GUI_element
	{
	public:
		TextRenderer(Font* font, RenderEngine::ShaderProgram* shader,
			std::string text, glm::vec3 color, glm::vec2 pos, glm::vec2 scale,
			std::string name = "default", bool isCenterCoord = true);

		TextRenderer(Font* font, RenderEngine::ShaderProgram* shader,
			std::wstring text, glm::vec3 color, glm::vec2 pos, glm::vec2 scale,
			std::string name = "default", bool isCenterCoord = true);
		~TextRenderer();

		void render_text(std::wstring text, float x, float y, float scale, glm::vec3 color, float layer, glm::mat4& prj);

		void on_render_prj(glm::mat4 prj) override;

		void set_text(std::string text);
		void set_text(std::wstring text);

		void set_position(glm::vec2 pos) override;
		void set_scale(glm::vec2 scale) override;

		inline float get_count_lines() { return m_lines.size(); }
	private:
		Font* m_font;
		RenderEngine::ShaderProgram* m_shader;

		std::vector<std::wstring> m_lines;

		std::wstring m_text;
		glm::vec3 m_color;

		glm::vec2 m_p_pos;

		RenderEngine::VertexArray* m_vertexArray;
		RenderEngine::VertexBuffer* m_vertexCoordsBuffer;
		unsigned int VAO, VBO;
		bool m_isCenterCoords;

		float m_y_shift;
	};
}
