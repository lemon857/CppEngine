#pragma once

#include "EngineCore/GUI/GUI_element.h"

namespace RenderEngine
{
	class Line;
}

namespace GUI
{
	class Bar : public GUI_element
	{
	public:
		Bar(std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec2 pos_p, glm::vec2 scale_p,
			float size_line, double max_value, glm::vec3 background_color, glm::vec3 foreground_color, std::string name);
		~Bar();

		void on_render_prj(glm::mat4& prj) override;

		void set_value(const double value);
		void set_max_value(const double max_val);
	private:
		float m_size;
		double m_max_value;
		double m_value;

		glm::vec3 m_background_color;
		glm::vec3 m_foreground_color;

		RenderEngine::Line* m_background_line;
		RenderEngine::Line* m_foreground_line;
	};
}