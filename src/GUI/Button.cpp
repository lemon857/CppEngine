#include "EngineCore/GUI/Button.h"

#include "EngineCore/GUI/GUI_place.h"
#include "EngineCore/GUI/TextRenderer.h"
#include "EngineCore/GUI/Font.h"
#include "EngineCore/GUI/Sprite.h"

#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Sound/Sound.h"

namespace GUI
{
	static unsigned int g_current_button_ID = 0;
	Button::Button(Sprite* face, glm::vec2 pos, glm::vec2 scale,
		std::string text, RenderEngine::ShaderProgram* textShader, Font* font, glm::vec3 textColor, std::string name)
		: GUI_element(name == "default" ? text : name)
		, m_textRenderer(new TextRenderer(font, std::move(textShader), text, textColor, glm::vec2(pos.x, pos.y - SHIFT_TEXT_SYMBOL_Y), glm::vec2(scale.x, 0.5f), m_name + "-text"))  // font sclae here
		, m_face(std::move(face))
	{
		m_isClickable = true;
		m_position_p = pos;
		m_scale_p = scale;
		m_face->set_position_p(pos);
		m_face->set_scale_p(scale);
	}
	Button::Button(Sprite* face, glm::vec2 pos, glm::vec2 scale, std::wstring text,
		RenderEngine::ShaderProgram* textShader, Font* font,
		glm::vec3 textColor, std::string name)
		: GUI_element(name == "default" ? "Button" + std::to_string(g_current_button_ID++) : name)
		, m_textRenderer(new TextRenderer(font, std::move(textShader), text, textColor, glm::vec2(pos.x, pos.y - SHIFT_TEXT_SYMBOL_Y), glm::vec2(scale.x, 0.5f), m_name + "-text"))  // font sclae here
		, m_face(std::move(face))
	{
		m_isClickable = true;
		m_position_p = pos;
		m_scale_p = scale;
		m_face->set_position_p(pos);
		m_face->set_scale_p(scale);
	}
	Button::~Button()
	{

	}
	void Button::on_render_prj(glm::mat4 prj)
	{

	}
	void Button::on_press()
	{
		m_face->setSubTexture(NAME_TEXTURE_CLICKED);
		//ResourceManager::get_sound("click")->play();
		m_isClicked = true;
		on_mouse_down();
	}
	void Button::on_release()
	{
		m_face->setSubTexture(NAME_TEXTURE_STATIC);
	}
	void Button::on_release_hover()
	{
		if (m_isClicked)
		{
			on_mouse_up();
			m_isClicked = false;
		}
	}
	std::vector<GUI_element*> Button::get_elements()
	{
		add_tree_element(m_face);
		add_tree_element(m_textRenderer);
		m_textRenderer->set_layer(m_layer + 2);
		m_face->set_layer(m_layer);
		std::vector<GUI_element*> vec;
		vec.push_back(m_textRenderer);
		vec.push_back(m_face);
		return vec;
	}

	void Button::set_text(std::string text)
	{
		m_textRenderer->set_text(text);
		//m_textRenderer->set_position(glm::vec2(m_position.x, m_position.y - GUI_place::get_pix_percent(glm::vec2(0.f, SHIFT_TEXT_SYMBOL_Y)).y));
	}
	void Button::set_text(std::wstring text)
	{
		m_textRenderer->set_text(text);
		//m_textRenderer->set_position(glm::vec2(m_position.x, m_position.y - GUI_place::get_pix_percent(glm::vec2(0.f, SHIFT_TEXT_SYMBOL_Y)).y));
	}
}
