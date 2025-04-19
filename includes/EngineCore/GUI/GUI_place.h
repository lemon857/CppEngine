#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include "EngineCore/System/Log.h"

#include "EngineCore/GUI/GUI_element.h"

#include "EngineCore/Renderer/Camera.h"

#include "EngineCore/Renderer/Material.h"

#include "EngineCore/System/Keys.h"


namespace GUI
{
	class GUI_place
	{
	public:
		GUI_place(Camera* render_cam, RenderEngine::Material* pMaterial);
		~GUI_place() = default; // need add trash clear

		void on_update(const double delta);

		void on_render();

		void add_element(GUI_element* element);

		template <class _Ty, class... _Types>
		_Ty* add_element(_Types&&... _Args)
		{
			GUI_element* element = (GUI_element*)(new _Ty(std::forward<_Types>(_Args)...));
			glm::vec2 posp = element->get_position_p();
			glm::vec2 scalep = element->get_scale_p();
			glm::vec2 wsize = m_render_cam->get_viewport_size();
			element->set_position(glm::vec2(posp.x / 100 * wsize.x, posp.y / 100 * wsize.y));
			element->set_scale(glm::vec2(scalep.x / 100 * wsize.x, scalep.y / 100 * wsize.y));
			m_els.push_back(element);
			add_elements(element->get_elements());
			m_elements.emplace(element->get_name(), element);
			std::string a = typeid(_Ty).name();
			if (a.find("Slider") != -1)	{
				m_els_needs_on_mouse_move.push_back(element);
			} else if (a.find("InputField") != -1) {
				m_els_needs_on_btn_press.push_back(element);
				m_els_needs_on_char_set.push_back(element);
			} else if (a.find("BindButton") != -1) {
				m_els_needs_on_btn_press.push_back(element);
			} else if (a.find("ChatBox") != -1 || a.find("ScrollBox") != -1) {
				m_els_needs_on_scroll.push_back(element);
			}
			return (_Ty*)element;
		}

		template <class _Ty, class... _Types>
		_Ty* add_element(float layer, _Types&&... _Args)
		{
			GUI_element* element = (GUI_element*)(new _Ty(std::forward<_Types>(_Args)...));
			glm::vec2 posp = element->get_position_p();
			glm::vec2 scalep = element->get_scale_p();
			glm::vec2 wsize = m_render_cam->get_viewport_size();
			element->set_position(glm::vec2(posp.x / 100 * wsize.x, posp.y / 100 * wsize.y));
			element->set_scale(glm::vec2(scalep.x / 100 * wsize.x, scalep.y / 100 * wsize.y));
			m_els.push_back(element);
			add_elements(element->get_elements());
			m_elements.emplace(element->get_name(), element);
			element->set_layer(layer);
			std::string a = typeid(_Ty).name();
      if (a.find("Slider") != -1)	{
				m_els_needs_on_mouse_move.push_back(element);
			} else if (a.find("InputField") != -1) {
				m_els_needs_on_btn_press.push_back(element);
				m_els_needs_on_char_set.push_back(element);
			} else if (a.find("BindButton") != -1) {
				m_els_needs_on_btn_press.push_back(element);
			} else if (a.find("ChatBox") != -1 || a.find("ScrollBox") != -1) {
				m_els_needs_on_scroll.push_back(element);
			}
			return (_Ty*)element;
		}

		template <class _Ty, class... _Types>
		_Ty* add_element(int layer, _Types&&... _Args)
		{
			GUI_element* element = (GUI_element*)(new _Ty(std::forward<_Types>(_Args)...));
			glm::vec2 posp = element->get_position_p();
			glm::vec2 scalep = element->get_scale_p();
			glm::vec2 wsize = m_render_cam->get_viewport_size();
			element->set_position(glm::vec2(posp.x / 100 * wsize.x, posp.y / 100 * wsize.y));
			element->set_scale(glm::vec2(scalep.x / 100 * wsize.x, scalep.y / 100 * wsize.y));
			m_els.push_back(element);
			add_elements(element->get_elements());
			m_elements.emplace(element->get_name(), element);
			element->set_layer(layer);
			std::string a = typeid(_Ty).name();			
			if (a.find("Slider") != -1)	{
				m_els_needs_on_mouse_move.push_back(element);
			} else if (a.find("InputField") != -1) {
				m_els_needs_on_btn_press.push_back(element);
				m_els_needs_on_char_set.push_back(element);
			} else if (a.find("BindButton") != -1) {
				m_els_needs_on_btn_press.push_back(element);
			} else if (a.find("ChatBox") != -1 || a.find("ScrollBox") != -1) {
				m_els_needs_on_scroll.push_back(element);
			}
			return (_Ty*)element;
		}

		template <class _Ty, class... _Types>
		_Ty* add_element(GUI_element* tree_parent, _Types&&... _Args)
		{
			GUI_element* element = (GUI_element*)(new _Ty(std::forward<_Types>(_Args)...));
			glm::vec2 posp = element->get_position_p();
			glm::vec2 scalep = element->get_scale_p();
			glm::vec2 wsize = m_render_cam->get_viewport_size();
			element->set_position(glm::vec2(posp.x / 100 * wsize.x, posp.y / 100 * wsize.y));
			element->set_scale(glm::vec2(scalep.x / 100 * wsize.x, scalep.y / 100 * wsize.y));
			m_els.push_back(element);
			add_elements(element->get_elements(), element);
			tree_parent->add_tree_element(element);
			m_elements.emplace(element->get_name(), element);
			std::string a = typeid(_Ty).name();			
			if (a.find("Slider") != -1)	{
				m_els_needs_on_mouse_move.push_back(element);
			} else if (a.find("InputField") != -1) {
				m_els_needs_on_btn_press.push_back(element);
				m_els_needs_on_char_set.push_back(element);
			} else if (a.find("BindButton") != -1) {
				m_els_needs_on_btn_press.push_back(element);
			} else if (a.find("ChatBox") != -1 || a.find("ScrollBox") != -1) {
				m_els_needs_on_scroll.push_back(element);
			}
			return (_Ty*)element;
		}	
		
		template <class _Ty, class... _Types>
		_Ty* add_element(GUI_element* tree_parent, float layer, _Types&&... _Args)
		{
			GUI_element* element = (GUI_element*)(new _Ty(std::forward<_Types>(_Args)...));
			glm::vec2 posp = element->get_position_p();
			glm::vec2 scalep = element->get_scale_p();
			glm::vec2 wsize = m_render_cam->get_viewport_size();
			element->set_position(glm::vec2(posp.x / 100 * wsize.x, posp.y / 100 * wsize.y));
			element->set_scale(glm::vec2(scalep.x / 100 * wsize.x, scalep.y / 100 * wsize.y));
			m_els.push_back(element);
			add_elements(element->get_elements(), element);
			tree_parent->add_tree_element(element);
			m_elements.emplace(element->get_name(), element);
			element->set_layer(layer);
			std::string a = typeid(_Ty).name();
			if (a.find("Slider") != -1)	{
				m_els_needs_on_mouse_move.push_back(element);
			} else if (a.find("InputField") != -1) {
				m_els_needs_on_btn_press.push_back(element);
				m_els_needs_on_char_set.push_back(element);
			} else if (a.find("BindButton") != -1) {
				m_els_needs_on_btn_press.push_back(element);
			} else if (a.find("ChatBox") != -1 || a.find("ScrollBox") != -1) {
				m_els_needs_on_scroll.push_back(element);
			}
			return (_Ty*)element;
		}

		template <class _Ty, class... _Types>
		_Ty* add_element(GUI_element* tree_parent, int layer, _Types&&... _Args)
		{
			GUI_element* element = (GUI_element*)(new _Ty(std::forward<_Types>(_Args)...));
			glm::vec2 posp = element->get_position_p();
			glm::vec2 scalep = element->get_scale_p();
			glm::vec2 wsize = m_render_cam->get_viewport_size();
			element->set_position(glm::vec2(posp.x / 100 * wsize.x, posp.y / 100 * wsize.y));
			element->set_scale(glm::vec2(scalep.x / 100 * wsize.x, scalep.y / 100 * wsize.y));
			m_els.push_back(element);
			add_elements(element->get_elements(), element);
			tree_parent->add_tree_element(element);
			m_elements.emplace(element->get_name(), element);
			element->set_layer(layer);
			std::string a = typeid(_Ty).name();
			if (a.find("Slider") != -1)	{
				m_els_needs_on_mouse_move.push_back(element);
			} else if (a.find("InputField") != -1) {
				m_els_needs_on_btn_press.push_back(element);
				m_els_needs_on_char_set.push_back(element);
			} else if (a.find("BindButton") != -1) {
				m_els_needs_on_btn_press.push_back(element);
			} else if (a.find("ChatBox") != -1 || a.find("ScrollBox") != -1) {
				m_els_needs_on_scroll.push_back(element);
			}
			return (_Ty*)element;
		}

		template<class _Ty>
		_Ty* get_element(std::string name) 
		{
			std::multimap<std::string, GUI_element*>::iterator it = m_elements.find(name);
			if (it != m_elements.end())
			{
				return (_Ty*)(it->second);
			}
			LOG_ERROR("[GUI] Can't find element: {0}", name);
			return nullptr;
		}

		void on_mouse_press(int x, int y);
		void on_mouse_release(int x, int y);
		void on_resize();
		void on_mouse_move(int x, int y);
		void on_mouse_scroll(int offset);
		void on_key_press(KeyCode key);
		void on_char_set(wchar_t key_char);


		bool get_focus();
		void set_logging_active(bool active);
		void set_active(bool active);

		static glm::vec2 get_pix_percent(glm::vec2 percent);

		static glm::vec2 get_vp_size();
		static glm::mat4 get_prj_matrix();

	private:
		void add_elements(std::vector<GUI_element*> elements);
		void add_elements(std::vector<GUI_element*> elements, GUI_element* tree_parent);

		static glm::vec2 m_vp_size;

		static glm::mat4 m_prj_mat;

		Camera* m_render_cam;
		std::shared_ptr<RenderEngine::Material> m_pMaterial;
		
		std::multimap<std::string, GUI_element*> m_elements;

		std::vector<GUI_element*> m_els;

		std::vector<GUI_element*> m_els_needs_on_mouse_move;
		std::vector<GUI_element*> m_els_needs_on_scroll;
		std::vector<GUI_element*> m_els_needs_on_char_set;
		std::vector<GUI_element*> m_els_needs_on_btn_press;

		bool m_isFocus = false;
		bool m_isActive = false;
		bool m_is_event_logging_active = false;
	};
}
