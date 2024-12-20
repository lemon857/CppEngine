#pragma once

#include "EngineCore/GUI/GUI_element.h"

namespace GUI
{
	class Sprite;

	class Slider : public GUI_element
	{
	public:
		Slider(Sprite* background, Sprite* slider, glm::vec2 pos, glm::vec2 scale, float min, float max, std::string name = "default");
		~Slider();

		void on_render_prj(glm::mat4 prj) override;

		void on_press() override;
		void on_release() override;

		void on_mouse_move(int x, int y) override;
		void set_active(const bool state) override;

		float value();

		void set_value(float val);

		std::vector<GUI_element*> get_elements() override;

		void set_slide_callback(std::function<void(float)> callback);
		void set_load_callback(std::function<void(float&)> callback);
	private:
		float m_min;
		float m_max;
		float m_value;

		bool is_pressed;

		Sprite* m_background;
		Sprite* m_slider;

		std::function<void(float)> m_slide_callback;
		std::function<void(float&)> m_load_callback;
	};
}
