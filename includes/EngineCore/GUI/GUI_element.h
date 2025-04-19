#pragma once

#include <string>
#include <vector>
#include <functional>

#include <glm/mat4x4.hpp>

// when you're set position, this is set center text on X coord
#define SHIFT_TEXT_SYMBOL_R_X 20.5f

#define SHIFT_TEXT_SYMBOL_X 10.0f
#define SHIFT_TEXT_SYMBOL_Y 1.2f

#define TEXT_SYMBOL_X 1.f

#define SHIFT_TEXT_SYMBOL_X_P 1.0f
#define SHIFT_TEXT_SYMBOL_Y_P 1.0f

#define NAME_TEXTURE_STATIC "static"
#define NAME_TEXTURE_CLICKED "clicked"

namespace RenderEngine
{
	class Material;
}

enum class KeyCode;

namespace GUI
{
	class GUI_element
	{
	public:

		// require stay material prop first in all childrens
		GUI_element(std::string name, RenderEngine::Material* pMaterial = nullptr)
			: m_name(name)
			, m_pMaterial(std::move(pMaterial))
			, m_position_p(glm::vec2(0))
			, m_scale_p(glm::vec2(0))
			, m_position(glm::vec2(0))
			, m_scale(glm::vec2(0))
			, m_position_mouse_click(glm::vec2(0))
			, m_isHovered(false)
			, m_isActive(true)
			, m_isFocused(false)
			, m_isClickable(false)
			, m_isFocusable(true)
			, m_layer(0)
		{
		}

		~GUI_element() = default;

		virtual void on_update(const double delta) {}
		virtual void on_render_prj(glm::mat4 prj) {}

		virtual void on_press() {};
		virtual void on_release() {};
		virtual void on_release_hover() {};
		virtual void on_mouse_move(int x, int y){}
		virtual void on_mouse_scroll(int offset){}
		virtual void on_key_press(KeyCode key){}
		virtual void on_char_set(wchar_t key_char) {}

		virtual void set_active(const bool state) { m_isActive = state; set_tree_active(state); }


		void set_focus(const bool state) { m_isFocused = state; }
		void set_focusable(const bool state) { m_isFocusable = state; }
		
		void switch_active() { m_isActive = !m_isActive; set_tree_active(m_isActive); };

		void set_layer(float layer) { m_layer = layer; set_tree_layer(layer); }

		virtual void add_position(glm::vec2 pos) { m_position += pos; set_tree_pos(m_position); }

		virtual void set_position(glm::vec2 pos) { m_position = pos; set_tree_pos(pos); }
		virtual void set_scale(glm::vec2 scale) { m_scale = scale; }

		GUI_element* add_tree_element(GUI_element* element) 
		{
			m_tree.push_back(element); 
			set_tree_active(m_isActive); 
			set_tree_focusable(m_isFocusable); 
			return std::move(element);
		}

		void set_tree_layer(float layer)
		{
			for (auto& i : m_tree)
			{
				i->set_layer(layer);
			}
		}
		void set_tree_active(const bool state)
		{
			for (auto& i : m_tree)
			{
				i->set_active(state);
			}
		}
		void set_tree_pos(const glm::vec2 pos)
		{
			for (auto& i : m_tree)
			{
				i->set_position(pos);
			}
		}void set_tree_focusable(const bool state)
		{
			for (auto& i : m_tree)
			{
				i->set_focusable(state);
			}
		}

		void set_position_p(glm::vec2 pos) { m_position_p = pos; }
		void set_scale_p(glm::vec2 scale) { m_scale_p = scale; }

		void set_pos_mouse_click(glm::vec2 pos) { m_position_mouse_click = pos; }

		int get_layer() { return m_layer; }

		glm::vec2 get_position() { return m_position; }
		glm::vec2 get_scale() { return m_scale; }

		glm::vec2 get_position_p() { return m_position_p; }
		glm::vec2 get_scale_p() { return m_scale_p; }

		glm::vec2 get_mouse_click_pos() { return m_position_mouse_click; }

		std::string get_name() { return m_name; }

		bool get_active() { return m_isActive; }
		bool get_focusable () { return m_isFocusable; }
		bool get_focus() { return m_isFocused; }
		bool is_clickable () { return m_isClickable; }

		void set_mouse_down_callback(std::function<void()> on_mouse_down) { m_on_mouse_down = on_mouse_down; }
		void set_mouse_up_callback(std::function<void()> on_mouse_up) { m_on_mouse_up = on_mouse_up; }

		RenderEngine::Material* get_material() { return m_pMaterial; }

		virtual std::vector<GUI_element*> get_elements() { return std::vector<GUI_element*>(); }
	protected:

		void on_mouse_up() { if (m_on_mouse_up != nullptr) m_on_mouse_up(); }
		void on_mouse_down() { if (m_on_mouse_down != nullptr) m_on_mouse_down(); }

		bool m_isHovered;
		bool m_isFocused;
		bool m_isActive;

		bool m_isClickable;
		bool m_isFocusable;

		float m_layer;

		std::function<void()> m_on_mouse_down;
		std::function<void()> m_on_mouse_up;
		// Real coords
		glm::vec2 m_position;
		glm::vec2 m_scale;
		// % screen range 0 100
		glm::vec2 m_position_p;
		glm::vec2 m_scale_p;

		glm::vec2 m_position_mouse_click;

		std::string m_name;
		RenderEngine::Material* m_pMaterial;

		std::vector<GUI_element*> m_tree;
	};
}
