#include "EngineCore/Renderer/Material.h"

#include "EngineCore/System/ShadersSettings.h"

#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Texture2D.h"
#include "EngineCore/Renderer/Renderer.h"

namespace RenderEngine
{
	Material::Material(ShaderProgram* pShaderProgram, Texture2D* pTexture)
		: m_pShaderProgram(std::move(pShaderProgram))
		, m_pTexture(std::move(pTexture))
	{
		m_shader_layout_elemrnts = m_pShaderProgram->get_layout()->getLayoutElements();
		for (const auto& curElements : m_shader_layout_elemrnts)
		{
			if (curElements.type > 5) continue;
			switch (curElements.type)
			{
			case Int:
				m_shader_data_map.emplace(curElements.name, new int[1] {0});
				break;
			case Float:
				m_shader_data_map.emplace(curElements.name, new float[1] {0});
				break;
			case Vec3:
				m_shader_data_map.emplace(curElements.name, new float[3] {0, 0, 0});
				break;
			case Vec4:
				m_shader_data_map.emplace(curElements.name, new float[4] {0, 0, 0, 0});
				break;
			case Col3:
				m_shader_data_map.emplace(curElements.name, new float[3] {0, 0, 0});
				break;
			case Col4:
				m_shader_data_map.emplace(curElements.name, new float[4] {0, 0, 0, 0});
				break;
			}
		}
	}

	Material::~Material()
	{
		if (!m_shader_data_map.empty())
		{
			for (auto curElement : m_shader_layout_elemrnts)
			{
        std::map<std::string, void*>::iterator it = m_shader_data_map.find(curElement.name);
        if (it == m_shader_data_map.end()) {
          continue;
        }
        if (it->second == nullptr) {
          continue;
        }
        switch (curElement.type)
        {
        case Int:
          delete[] (int*)(it->second);
          break;
        case Float:
        case Vec3:
        case Vec4:
        case Col3:
        case Col4:
          delete[] (float*)(it->second);
          break;
        }
			}
			m_shader_data_map.clear();
		}
	}

	void Material::use()
	{
		m_pShaderProgram->use();

		void* data;

		for (const auto& curElements : m_shader_layout_elemrnts)
		{
			data = get_data<void>(curElements.name);
			switch (curElements.type)
			{
			case Int:
				m_pShaderProgram->setInt(curElements.name, ((int*)data)[0]);
				break;
			case Float:
				m_pShaderProgram->setFloat(curElements.name, ((float*)data)[0]);
				break;
			case Vec3:
				m_pShaderProgram->setVec3(curElements.name, glm::vec3(((float*)data)[0], ((float*)data)[1], ((float*)data)[2]));
				break;
			case Vec4:
				m_pShaderProgram->setVec4(curElements.name, glm::vec4(((float*)data)[0], ((float*)data)[1], ((float*)data)[2], ((float*)data)[3]));
				break;
			case Col3:
				m_pShaderProgram->setVec3(curElements.name, glm::vec3(((float*)data)[0], ((float*)data)[1], ((float*)data)[2]));
				break;
			case Col4:
				m_pShaderProgram->setVec4(curElements.name, glm::vec4(((float*)data)[0], ((float*)data)[1], ((float*)data)[2], ((float*)data)[3]));
				break;
			}
		}

		if (m_pTexture != nullptr) Renderer::bindTexture(*m_pTexture);
	}
	std::vector<std::string> Material::get_shader_prop_names(ETypeData type)
	{
		std::vector<std::string> names;

		auto elements = m_pShaderProgram->get_layout()->getLayoutElements();

		for (const auto& curElement : elements)
		{
			if (curElement.type == type) names.push_back(curElement.name);
		}

		return names;
	}

	void Material::set_model_matrix(glm::mat4& model)
	{
		m_pShaderProgram->setMatrix4(SS_MODEL_MATRIX_NAME, model);
	}
	void Material::set_view_projection_matrix(glm::mat4& vp)
	{
		m_pShaderProgram->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, vp);
	}
	Texture2D* Material::get_texture_ptr()
	{
		return m_pTexture;
	}
	ShaderProgram* Material::get_shader_ptr()
	{
		return m_pShaderProgram;
	}
	void Material::set_texture(Texture2D* pTexture)
	{
		m_pTexture = std::move(pTexture);
	}
	void Material::set_shader(ShaderProgram* pShaderProgram)
	{
		m_pShaderProgram = std::move(pShaderProgram);

		if (!m_shader_data_map.empty())
		{
			for (auto curElement : m_shader_layout_elemrnts)
			{
        std::map<std::string, void*>::iterator it = m_shader_data_map.find(curElement.name);
        if (it == m_shader_data_map.end()) {
          continue;
        }
        if (it->second == nullptr) {
          continue;
        }
        switch (curElement.type)
        {
        case Int:
          delete[] (int*)(it->second);
          break;
        case Float:
        case Vec3:
        case Vec4:
        case Col3:
        case Col4:
          delete[] (float*)(it->second);
          break;
        }
			}
			m_shader_data_map.clear();
		}
		m_shader_layout_elemrnts = m_pShaderProgram->get_layout()->getLayoutElements();
		for (const auto& curElements : m_shader_layout_elemrnts)
		{
			if (curElements.type > 5) continue;
			switch (curElements.type)
			{
			case Int:
				m_shader_data_map.emplace(curElements.name, new int[1] {0});
				break;
			case Float:
				m_shader_data_map.emplace(curElements.name, new float[1] {0});
				break;
			case Vec3:
				m_shader_data_map.emplace(curElements.name, new float[3] {0, 0, 0});
				break;
			case Vec4:
				m_shader_data_map.emplace(curElements.name, new float[4] {0, 0, 0, 0});
				break;
			case Col3:
				m_shader_data_map.emplace(curElements.name, new float[3] {0, 0, 0});
				break;
			case Col4:
				m_shader_data_map.emplace(curElements.name, new float[4] {0, 0, 0, 0});
				break;
			}
		}
	}	
}