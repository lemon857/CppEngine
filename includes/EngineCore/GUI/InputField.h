#pragma once

#include "EngineCore/GUI/GUI_element.h"

#include <string>
#include <memory>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

enum class KeyCode;

namespace RenderEngine
{
	class Material;
	class ShaderProgram;
}

namespace GUI
{
	class TextRenderer;
	class Font;
	class Sprite;

	class InputField : public GUI_element
	{
	public:
		InputField(Sprite* face, glm::vec2 pos, glm::vec2 scale,
			std::string name, RenderEngine::ShaderProgram* shader, 
			Font* font, glm::vec3 textColor, bool clear_after_send = false, bool* focus_trigger = nullptr);
		~InputField();

		void on_render_prj(glm::mat4 prj) override;

		void on_press() override;
		void on_release() override;

		void set_active(const bool state) override;

		std::vector<GUI_element*> get_elements() override;

		std::wstring get_text();
		void set_text(std::wstring text);

		bool get_focus();
		void set_focus(bool focus);

		void on_key_press(KeyCode key) override;
		void on_char_set(wchar_t key_char) override;

		void set_enter_callback(std::function<void(std::wstring text)> on_enter) { m_on_enter = on_enter; }
	private:
		std::function<void(std::wstring text)> m_on_enter;

		Sprite* m_face;
		TextRenderer* m_textRenderer;
		std::wstring m_text;

		bool* m_focus_trigger;
		bool m_isClicked;
		bool m_clear_after_send;
	};
}
