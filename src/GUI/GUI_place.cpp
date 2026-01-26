#include "EngineCore/GUI/GUI_place.h"
#include "EngineCore/GUI/GUI_element.h"

#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer/ShaderProgram.h"

#include "EngineCore/System/ShadersSettings.h"
#include "EngineCore/Renderer/Camera.h"

#include "EngineCore/System/Log.h"

namespace GUI
{
	glm::vec2 GUI_place::m_vp_size;
	glm::mat4 GUI_place::m_prj_mat;

	GUI_place::GUI_place(Camera* render_cam, RenderEngine::Material* pMaterial)
		: m_pMaterial(pMaterial)
		, m_render_cam(render_cam)
	{
		m_vp_size = m_render_cam->get_viewport_size();
		m_prj_mat = m_render_cam->get_ui_matrix();

		m_is_event_logging_active = true;
#ifndef DEBUG
		m_is_event_logging_active = true;
#endif
	}

  GUI_place::~GUI_place() {
    // for (auto& i : m_elements) {
    //   delete i.second; 
    // }
  }

	void GUI_place::on_update(const double delta)
	{
		if (!m_isActive) return;
		for (auto cur : m_els)
		{
			cur->on_update(delta);
		}
	}
	void GUI_place::on_render()
	{
		if (!m_isActive) return;
		//if (!m_isActive) return;
		//m_pMaterial->use();
		//m_pMaterial->get_shader_ptr()->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_render_cam->get_ui_matrix());
		//glm::vec2 size = m_render_cam->get_viewport_size();
		for (auto cur : m_els)
		{
			cur->on_render_prj(m_prj_mat);
		}
	}
	void GUI_place::add_element(GUI_element* element)
	{		
		glm::vec2 posp = element->get_position_p();
		glm::vec2 scalep = element->get_scale_p();
		m_vp_size = m_render_cam->get_viewport_size();
		element->set_position(glm::vec2(posp.x / 100 * m_vp_size.x, m_vp_size.y / 100 * m_vp_size.y));
		element->set_scale(glm::vec2(scalep.x / 100 * m_vp_size.x, m_vp_size.y / 100 * m_vp_size.y));
		m_els.push_back(element);
		add_elements(element->get_elements());
		m_elements.emplace(element->get_name(), element);
	}
	void GUI_place::on_resize()
	{
		m_prj_mat = m_render_cam->get_ui_matrix();
		m_vp_size = m_render_cam->get_viewport_size();
		for (auto cur : m_els)
		{
			glm::vec2 posp = cur->get_position_p();
			glm::vec2 scalep = cur->get_scale_p();
			cur->set_position(glm::vec2(posp.x / 100 * m_vp_size.x, posp.y / 100 * m_vp_size.y));
			cur->set_scale(glm::vec2(scalep.x / 100 * m_vp_size.x, scalep.y / 100 * m_vp_size.y));
		}
	}
	void GUI_place::on_mouse_move(int x, int y)
	{
		if (!m_isActive) return;
		for (auto& i : m_els_needs_on_mouse_move)
		{
			i->on_mouse_move(x, y);
		}
	}
	void GUI_place::on_mouse_scroll(int offset)
	{
		if (!m_isActive) return;
		for (auto& i : m_els_needs_on_scroll)
		{
			i->on_mouse_scroll(offset);
		}
	}
	void GUI_place::on_key_press(KeyCode key)
	{
		if (!m_isActive) return;
		for (auto& i : m_els_needs_on_btn_press)
		{
			i->on_key_press(key);
		}
	}
	void GUI_place::on_char_set(wchar_t key_char)
	{
		if (!m_isActive) return;
		for (auto& i : m_els_needs_on_char_set)
		{
			i->on_char_set(key_char);
		}
	}
	bool GUI_place::get_focus()
	{
		return m_isFocus;
	}
	void GUI_place::set_logging_active(bool active)
	{
		m_is_event_logging_active = active;
	}
	void GUI_place::set_active(bool active)
	{
		m_isActive = active;
	}
	glm::vec2 GUI_place::get_pix_percent(glm::vec2 percent)
	{
		return glm::vec2(percent.x / 100 * m_vp_size.x, percent.y / 100 * m_vp_size.y);
	}
	glm::vec2 GUI_place::get_vp_size()
	{
		return m_vp_size;
	}
	glm::mat4 GUI_place::get_prj_matrix()
	{
		return m_prj_mat;
	}
	void GUI_place::add_elements(std::vector<GUI_element*> elements)
	{
		if (elements.empty()) return;
		for (auto cur : elements)
		{
			glm::vec2 posp = cur->get_position_p();
			glm::vec2 scalep = cur->get_scale_p();
			cur->set_position(glm::vec2(posp.x / 100 * m_vp_size.x, posp.y / 100 * m_vp_size.y));
			cur->set_scale(glm::vec2(scalep.x / 100 * m_vp_size.x, scalep.y / 100 * m_vp_size.y));
			m_elements.emplace(cur->get_name(), cur);
			m_els.push_back(cur);
			add_elements(cur->get_elements());
		}
	}
	void GUI_place::add_elements(std::vector<GUI_element*> elements, GUI_element* tree_parent)
	{
		if (elements.empty()) return;
		for (auto cur : elements)
		{
			glm::vec2 posp = cur->get_position_p();
			glm::vec2 scalep = cur->get_scale_p();
			cur->set_position(glm::vec2(posp.x / 100 * m_vp_size.x, posp.y / 100 * m_vp_size.y));
			cur->set_scale(glm::vec2(scalep.x / 100 * m_vp_size.x, scalep.y / 100 * m_vp_size.y));
			tree_parent->add_tree_element(cur);
			m_elements.emplace(cur->get_name(), cur);
			m_els.push_back(cur);
			add_elements(cur->get_elements());
		}
	}

	void GUI_place::on_mouse_press(int x, int y)
	{
		if (!m_isActive) return;
		m_vp_size = m_render_cam->get_viewport_size();
		y = m_vp_size.y - y; // set null pos in left down
		GUI_element* select = nullptr;
		for (auto cur : m_els)
		{
			if (!cur->get_active()) continue;
			glm::vec2 scale = cur->get_scale();
			glm::vec2 pos = cur->get_position() - scale;
			scale *= 2;
			if ((x >= pos.x && y >= pos.y && x <= pos.x + scale.x && y <= pos.y + scale.y))
			{
				if (select != nullptr)
				{
					if (cur->get_layer() > select->get_layer() && cur->is_clickable()) select = cur;
				}
				else if (cur->is_clickable()) select = cur;
				if (cur->get_focusable()) m_isFocus = true;
			}
		}
		if (select != nullptr)
		{
			select->set_pos_mouse_click(glm::vec2(x, y));
			select->on_press();
			if (m_is_event_logging_active) LOG_INFO("[GUI] Press mouse on object: {0}", select->get_name());
		}
	}

	void GUI_place::on_mouse_release(int x, int y)
	{
		if (!m_isActive) return;
		m_isFocus = false;
		m_vp_size = m_render_cam->get_viewport_size();
		y = m_vp_size.y - y; // set null pos in left down
		GUI_element* select = nullptr;
		for (auto cur : m_els)
		{
			cur->on_release();
			if (!cur->get_active()) continue;
			glm::vec2 scale = cur->get_scale();
			glm::vec2 pos = cur->get_position() - scale;
			scale *= 2;
			if ((x >= pos.x && y >= pos.y && x <= pos.x + scale.x && y <= pos.y + scale.y))
			{
				if (select != nullptr)
				{
					if (cur->get_layer() > select->get_layer() && cur->is_clickable()) select = cur;
				}
				else if (cur->is_clickable()) select = cur;
			}
		}
		if (select != nullptr)
		{
			select->on_release_hover();
			if (m_is_event_logging_active) LOG_INFO("[GUI] Release mouse on object: {0}", select->get_name());
		}
	}
}
