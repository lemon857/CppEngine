#include "EngineCore/GUI/Slider.h"
#include "EngineCore/GUI/Sprite.h"

#include "EngineCore/System/Log.h"

namespace GUI
{
	GUI::Slider::Slider(Sprite* background, Sprite* slider, glm::vec2 pos, glm::vec2 scale, float min, float max, std::string name)
		: GUI_element(name)
		, m_background(std::move(background))
		, m_slider(std::move(slider))
		, m_min(min)
		, m_max(max)
		, is_pressed(false)
		, m_load_callback(nullptr)
		, m_slide_callback(nullptr)
	{
		m_isClickable = true;
		m_position_p = pos;
		m_scale_p = scale;
		m_background->set_position_p(pos);
		m_background->set_scale_p(scale);
		m_slider->set_position_p(glm::vec2(pos.x - scale.x, pos.y));
		m_slider->set_scale_p(glm::vec2(scale.x / 3, scale.y));
	}
	Slider::~Slider()
	{
	}
	void Slider::on_render_prj(glm::mat4 prj)
	{
	}
	void Slider::on_press()
	{
		m_slider->setSubTexture(NAME_TEXTURE_CLICKED);
		is_pressed = true; 
		float y = (float)(m_position_mouse_click.x - m_position.x + m_scale.x) / (float)(m_scale.x * 2.f);
		m_value = y * (float)(m_max - m_min);
		if (m_value < m_min)
		{
			m_value = m_min;
			return;
		}
		else if (m_value > m_max)
		{
			m_value = m_max;
			return;
		}
		m_slider->set_position(glm::vec2(y * (m_scale.x + m_scale.x) + m_position.x - m_scale.x, m_position.y));
		if (m_slide_callback != nullptr) m_slide_callback(m_value);
	}
	void Slider::on_release()
	{
		m_slider->setSubTexture(NAME_TEXTURE_STATIC);
		is_pressed = false;
	}
	void Slider::on_mouse_move(int x, int y)
	{
		if (is_pressed)
		{
			float y = (float)(x - m_position.x + m_scale.x) / (float)(m_scale.x * 2.f);
			m_value = y * (float)(m_max - m_min);
			if (m_value < m_min)
			{
				m_value = m_min;
				return;
			}
			else if (m_value > m_max)
			{
				m_value = m_max;
				return;
			}
			m_slider->set_position(glm::vec2(y * (m_scale.x + m_scale.x) + m_position.x - m_scale.x, m_position.y));
			if (m_slide_callback != nullptr) m_slide_callback(m_value);
			//LOG_INFO("[SLIDER] Val: {0}", m_value);
		}
	}
	void Slider::set_active(const bool state)
	{
		m_isActive = state;
		set_tree_active(state);
		if (state && m_load_callback != nullptr) m_load_callback(m_value);
	}
	float Slider::value()
	{
		return m_value;
	}
	void Slider::set_value(float val)
	{
		m_value = val;
		float y = val / (float)(m_max - m_min);
		m_slider->set_position(glm::vec2(y * (m_scale.x + m_scale.x) + m_position.x - m_scale.x, m_position.y));
	}
	std::vector<GUI_element*> Slider::get_elements()
	{
		add_tree_element(m_slider);
		add_tree_element(m_background);
		std::vector<GUI_element*> vec;
		vec.push_back(m_slider);
		vec.push_back(m_background);
		return vec;
	}
	void Slider::set_slide_callback(std::function<void(float)> callback)
	{
		m_slide_callback = callback;
	}
	void Slider::set_load_callback(std::function<void(float&)> callback)
	{
		m_load_callback = callback;
	}
}
