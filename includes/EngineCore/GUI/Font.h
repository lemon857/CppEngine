#pragma once

#include <glm/vec2.hpp>
#include <string>
#include <map>

namespace GUI
{
	struct Font_Glyph {
		unsigned int TextureID; // ID �������� �����
		glm::ivec2   Size;      // ������� �����
		glm::ivec2   Bearing;   // �������� ������� ����� ����� �����
		unsigned int Advance;   // �������������� �������� �� ������ ���������� �����
	};

	class Font
	{
	public:
		Font();
		~Font();

		bool load(std::string& font_path, unsigned int font_size);

		Font_Glyph get_glyph(const char sym);

	private:
		std::map<const char, Font_Glyph> m_glyphs;
	};
}