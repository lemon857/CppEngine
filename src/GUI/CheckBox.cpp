#include "EngineCore/GUI/CheckBox.h"

#include "EngineCore/GUI/Sprite.h"

#include "EngineCore/System/Log.h"

namespace GUI
{
	CheckBox::CheckBox(Sprite* backgroung, Sprite* checkFlag, glm::vec2 pos, glm::vec2 scale, std::string name)
		: GUI_element(name)
		, m_backgroung(std::move(backgroung))
		, m_check(std::move(checkFlag))
		, m_value(false)
		, m_isClicked(false)
	{
		m_position_p = pos;
		m_scale_p = scale;
		m_backgroung->set_position_p(pos);
		m_backgroung->set_scale_p(scale);
		m_check->set_position_p(pos);
		m_check->set_scale_p(scale);
	}

	CheckBox::~CheckBox()
	{
		delete m_backgroung;
		delete m_check;
	}

	void CheckBox::on_render_prj(glm::mat4 prj)
	{
		if (!m_isActive) return;
		if (m_value)
		{
			m_check->on_render_prj(prj);
		}		
		m_backgroung->on_render_prj(prj);		
	}

	void CheckBox::on_press()
	{
		on_mouse_down();
		m_isClicked = true;
	}

	void CheckBox::on_release()
	{
		if (m_isClicked)
		{
			m_value = !m_value;
			m_isClicked = false;
			//LOG_INFO("Value: {0}", m_value);
			on_mouse_up();
		}
	}

	std::vector<GUI_element*> CheckBox::get_elements()
	{
		add_tree_element(m_backgroung);
		add_tree_element(m_check);
		std::vector<GUI_element*> vec;
		vec.push_back(m_backgroung);
		vec.push_back(m_check);
		return vec;
	}

	bool CheckBox::value()
	{
		return m_value;
	}

}
