#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Texture2D.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"
#include "EngineCore/Renderer/Animation.h"
#include "EngineCore/Renderer/Animator.h"
#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"
#include "EngineCore/Renderer/ShaderProgramLayout.h"
#include "EngineCore/System/Log.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "EngineCore/Resources/stb_image.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_ShaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::MaterialsMap ResourceManager::m_materials;
ResourceManager::CacheOBJMap ResourceManager::m_obj_files;
//ResourceManager::SpriteRenderersMap ResourceManager::m_SpriteRenderers;
//ResourceManager::GraphObjMap ResourceManager::m_graph_objs;
//ResourceManager::AnimatorsMap ResourceManager::m_animators;
std::string ResourceManager::m_path;


void ResourceManager::unloadAllResources()
{
	m_ShaderPrograms.clear();
	m_textures.clear();
	m_materials.clear();
	m_obj_files.clear();
	//m_SpriteRenderers.clear();
}
void ResourceManager::setExecutablePath(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}
bool ResourceManager::load_JSON_resources(const std::string & JSONpath)
{
	const std::string JSONstr = getFileString(JSONpath);
	if (JSONstr.empty())
	{
		LOG_CRIT("No JSON file: {0}", JSONpath);
		return false;
	}
	rapidjson::Document doc;
	rapidjson::ParseResult result = doc.Parse(JSONstr.c_str());
	if (!result)
	{
		LOG_CRIT("JSON parse error: {0} ({1})", rapidjson::GetParseError_En(result.Code()),  result.Offset());
		LOG_CRIT("In JSON file: ", JSONpath);
		return false;
	}
	auto shadersIt = doc.FindMember("shaders");
	if (shadersIt != doc.MemberEnd())
	{
		for (const auto& currentShader : shadersIt->value.GetArray())
		{
			const std::string name = currentShader["name"].GetString();
			const std::string vertShader = currentShader["filePath_v"].GetString();
			const std::string fragShader = currentShader["filePath_f"].GetString();
			const auto layoutArray = currentShader["layout"].GetArray();

			std::shared_ptr<RenderEngine::ShaderProgramLayout> layout = std::make_shared<RenderEngine::ShaderProgramLayout>();
						
			for (const auto& currentLayoutElement : layoutArray)
			{
				const std::string name = currentLayoutElement["name"].GetString();
				const int type = currentLayoutElement["typeInt"].GetInt();
				const double min = currentLayoutElement["minVal"].GetDouble();
				const double max = currentLayoutElement["maxVal"].GetDouble();
				layout->addElementLayout(name, (ETypeData)type, min, max);
			}

			loadShaders(name, vertShader, fragShader, layout);
		}
	}
	auto atlasesIt = doc.FindMember("textureAtlases");
	if (atlasesIt != doc.MemberEnd())
	{
		for (const auto& currentAtlas : atlasesIt->value.GetArray())
		{
			const std::string name = currentAtlas["name"].GetString();
			const std::string path = currentAtlas["filePath"].GetString();
			const unsigned int width = currentAtlas["width"].GetUint();
			const unsigned int height = currentAtlas["height"].GetUint();
			const unsigned int subTextureWidth = currentAtlas["subTexture_w"].GetUint();
			const unsigned int subTextureHeight = currentAtlas["subTexture_h"].GetUint();

			const auto subTexturesArray = currentAtlas["subTextures"].GetArray();
			std::vector<std::string> subTextures;
			subTextures.reserve(subTexturesArray.Size());

			for (const auto& currentSubTexture : subTexturesArray)
			{
				subTextures.emplace_back(currentSubTexture.GetString());
			}

			loadTextureAtlas(name, subTextures, path, width, height, subTextureWidth, subTextureHeight);
		}
	}
	auto materialsIt = doc.FindMember("materials");
	if (materialsIt != doc.MemberEnd())
	{
		for (const auto& currentMaterial : materialsIt->value.GetArray())
		{
			const std::string name = currentMaterial["name"].GetString();
			const std::string shaderName = currentMaterial["nameShader"].GetString();
			const std::string textureName = currentMaterial["nameTexture"].GetString();

			loadMaterial(name, textureName, shaderName);
		}
	}
	/*auto SpriteRenderersIt = doc.FindMember("SpriteRenderers");
	if (SpriteRenderersIt != doc.MemberEnd())
	{
		for (const auto& currenSpriteRenderer : SpriteRenderersIt->value.GetArray())
		{
			const std::string name = currenSpriteRenderer["name"].GetString();
			const std::string shader = currenSpriteRenderer["shader"].GetString();
			const std::string atlas = currenSpriteRenderer["textureAtlas"].GetString();
			const std::string initSubTexture = currenSpriteRenderer["initialSubTexture"].GetString();

			loadSpriteRenderer(name, atlas, shader, initSubTexture);			
		}
	}
	auto objectsIt = doc.FindMember("objects");
	if (objectsIt != doc.MemberEnd())
	{
		for (const auto& currenObject : objectsIt->value.GetArray())
		{
			const std::string name = currenObject["name"].GetString();
			const std::string shader = currenObject["shader"].GetString();
			const std::string source = currenObject["source"].GetString();
			
			loadGraphicsObject(name, shader, source);
		}
	}*/
	LOG_INFO("Loadind data in JSON file complete");
	/*auto animatorsIt = doc.FindMember("animators");
	if (animatorsIt != doc.MemberEnd())
	{
		for (const auto& currenAnimator : animatorsIt->value.GetArray())
		{
			const std::string name = currenAnimator["name"].GetString();
			const std::string SpriteRendererName = currenAnimator["SpriteRendererName"].GetString();
			auto pAnimator = loadAnimator(name, SpriteRendererName);

			const auto animationsArray = currenAnimator["Animations"].GetArray();

			for (const auto& currentAnimation : animationsArray)
			{
				const std::string animationName = currentAnimation["name"].GetString();
				const int callbackAction = currentAnimation["callbackAction"].GetInt();
				const int typeAnimation = currentAnimation["typeAnimation"].GetInt();
				const auto subTextureNamesArray = currentAnimation["subTextureNames"].GetArray();
				const auto durationsArray = currentAnimation["durations"].GetArray();

				std::vector<std::string> subTextureNames;
				std::vector<double> durations;
				
				for (size_t i = 0; i < subTextureNamesArray.Size(); i++)
				{
					subTextureNames.push_back(subTextureNamesArray[i].GetString());
					durations.push_back(durationsArray[i].GetDouble());
				}
				
				pAnimator->addAnimation(animationName, std::make_shared<RenderEngine::Animation>(subTextureNames, durations, 
					RenderEngine::ECallbackAction(callbackAction), RenderEngine::ETypeAnimation(typeAnimation)));
			}
		}
	}*/
	return true;
}
bool ResourceManager::load_INI_settings(const std::string& INIpath, INIdata& data, const bool isWrite)
{
	std::ifstream f;
	f.open(m_path + "/" + INIpath, std::ios::in | std::ios::binary);

	if (f.is_open() && !isWrite)
	{

		std::stringstream buf;
		buf << f.rdbuf();
		f.close();

		std::string INIstr = buf.str();
		if (!INIstr.empty() && !isWrite)
		{
			const size_t newLine = INIstr.find('\n');
			const size_t newLine2 = INIstr.substr(newLine + 1, INIstr.size() - newLine + 1).find('\n');

			const std::string size = INIstr.substr(0, newLine - 1);
			const std::string pos = INIstr.substr(newLine + 1, newLine2 - 1);

			const std::string width = size.substr(0, size.find(' '));
			const std::string height = size.substr(size.find(' ') + 1, size.size());

			const std::string posX = pos.substr(0, pos.find(' '));
			const std::string posY = pos.substr(pos.find(' ') + 1, pos.size());

			const std::string maximized = INIstr.substr(newLine + newLine2 + 2, INIstr.size() - newLine2 - newLine);

			LOG_INFO("Succsess read file {0}", INIpath);
			LOG_INFO("Data {2} {0}x{1} {3}x{4}", width, height, maximized, posX, posY);

			data.window_size = glm::ivec2(std::stoi(width), std::stoi(height));
			data.window_position = glm::ivec2(std::stoi(posX), std::stoi(posY));
			data.maximized_window = std::stoi(maximized) == 1 ? true : false;
			return true;
		}
	}
	std::ofstream writeStream;
	writeStream.open(m_path + "/" + INIpath);

	if (writeStream.is_open())
	{
		LOG_INFO("Succsess open file {0} for write data", INIpath);
		writeStream << std::to_string(data.window_size.x) << " " << std::to_string(data.window_size.y) << "\n";
		writeStream << std::to_string(data.window_position.x) << " " << std::to_string(data.window_position.y) << "\n";
		writeStream << std::to_string(data.maximized_window == true ? 1 : 0);
		writeStream.close();
		return true;
	}
	else
	{
		LOG_ERROR("Error open file: {0}", INIpath);
		writeStream.close();
		return false;
	}
}
std::shared_ptr<GraphicsObject> ResourceManager::load_OBJ_file(const std::string& OBJrelativePath, bool is_reload)
{
	CacheOBJMap::iterator it = m_obj_files.find(OBJrelativePath);
		
	if (it != m_obj_files.end())
	{
		if (!is_reload) return it->second;
	}
	
	std::ifstream file;
	file.open(m_path + "/" + OBJrelativePath);
	if (file.is_open())
	{
		std::vector<GLfloat> vertex_coords;
		std::vector<GLfloat> normal_coords;
		std::vector<GLfloat> texture_coord;
		std::vector<GLuint> index_array;

		std::vector<glm::vec3> temp_pos;
		std::vector<glm::vec3> temp_norms;
		std::vector<glm::vec2> temp_texs;

		std::vector<GLuint> indices_coords;
		std::vector<GLuint> indices_norms;
		std::vector<GLuint> indices_texs;

		bool need_normalize_vertex_pos = false;

		std::string line;
		while (std::getline(file, line))
		{
			if (start_with(line, "v "))
			{
				GLfloat x, y, z;
				scanf(line.c_str(), "v %f %f %f", &x, &y, &z);
				temp_pos.push_back(glm::vec3(x, y, z));
				if (!need_normalize_vertex_pos && (x > 1.f || x < -1.f)) need_normalize_vertex_pos = true;
#ifdef DEBUG_CONSOLE_OBJ_LOAD
				LOG_INFO("Vert: {0}x{1}x{2}", x, y, z);
#endif // DEBUG_CONSOLE_OBJ_LOAD

			}
			else if (start_with(line, "vn "))
			{
				GLfloat x, y, z;
				scanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
				temp_norms.push_back(glm::vec3(x, y, z));
#ifdef DEBUG_CONSOLE_OBJ_LOAD
				LOG_INFO("Norms: {0}x{1}x{2}", x, y, z);
#endif // DEBUG_CONSOLE_OBJ_LOAD
			}
			else if (start_with(line, "vt "))
			{
				GLfloat x, y;
				scanf(line.c_str(), "vt %f %f", &x, &y);
				temp_texs.push_back(glm::vec2(x, y));
#ifdef DEBUG_CONSOLE_OBJ_LOAD
				LOG_INFO("Texs: {0}x{1}", x, y);
#endif // DEBUG_CONSOLE_OBJ_LOAD
			}
			// WARNING region (# faces) in .obj file must be lastest
			else if (start_with(line, "f "))
			{
				int indexX, textureX, normalX;
				int indexY, textureY, normalY;
				int indexZ, textureZ, normalZ;
				scanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
					&indexX, &textureX, &normalX, &indexY, &textureY, &normalY, &indexZ, &textureZ, &normalZ);

				indices_coords.push_back(indexX - 1);
				indices_coords.push_back(indexY - 1);
				indices_coords.push_back(indexZ - 1);

				indices_norms.push_back(normalX - 1);
				indices_norms.push_back(normalY - 1);
				indices_norms.push_back(normalZ - 1);

				indices_texs.push_back(textureX - 1);
				indices_texs.push_back(textureY - 1);
				indices_texs.push_back(textureZ - 1);

#ifdef DEBUG_CONSOLE_OBJ_LOAD
				LOG_INFO("Faces: {0}x{1}x{2} {3}x{4}x{5} {6}x{7}x{8}", indexX, textureX, normalX, indexY, textureY, normalY, indexZ, textureZ, normalZ);
#endif // DEBUG_CONSOLE_OBJ_LOAD
			}
		}

		for (size_t i = 0; i < indices_coords.size(); i++)
		{
			index_array.push_back(i);

			vertex_coords.push_back(temp_pos[indices_coords[i]].x);
			vertex_coords.push_back(temp_pos[indices_coords[i]].y);
			vertex_coords.push_back(temp_pos[indices_coords[i]].z);

			normal_coords.push_back(temp_norms[indices_norms[i]].x);
			normal_coords.push_back(temp_norms[indices_norms[i]].y);
			normal_coords.push_back(temp_norms[indices_norms[i]].z);

			texture_coord.push_back(temp_texs[indices_texs[i]].x);
			texture_coord.push_back(temp_texs[indices_texs[i]].y);
		}

		std::shared_ptr<RenderEngine::VertexArray> vao = std::make_shared<RenderEngine::VertexArray>();
		std::shared_ptr<RenderEngine::IndexBuffer> ebo = std::make_shared<RenderEngine::IndexBuffer>();

		RenderEngine::VertexBuffer vbo_vert;
		RenderEngine::VertexBuffer vbo_normal;
		RenderEngine::VertexBuffer vbo_texture;

		vbo_vert.init(vertex_coords.data(), vertex_coords.size() * sizeof(GLfloat), true);
		RenderEngine::VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		vao->addBuffer(vbo_vert, vertexCoordsLayout);

		vbo_normal.init(normal_coords.data(), normal_coords.size() * sizeof(GLfloat), true);
		RenderEngine::VertexBufferLayout normalCoordsLayout;
		normalCoordsLayout.addElementLayoutFloat(3, false);
		vao->addBuffer(vbo_normal, normalCoordsLayout);

		vbo_texture.init(texture_coord.data(), texture_coord.size() * sizeof(GLfloat), true);
		RenderEngine::VertexBufferLayout textureCoordsLayout;
		textureCoordsLayout.addElementLayoutFloat(2, false);
		vao->addBuffer(vbo_texture, textureCoordsLayout);

		ebo->init(index_array.data(), index_array.size() * sizeof(GLuint));

		vao->unbind();
		ebo->unbind();

		file.close();

		if (is_reload)
		{
			it->second = std::make_shared<GraphicsObject>(std::move(vao), std::move(ebo));

			LOG_INFO("Success load obj file: {0}", OBJrelativePath);
			return it->second;
		}
		else
		{
			std::shared_ptr<GraphicsObject> newOBJ =
				m_obj_files.emplace(OBJrelativePath, std::make_shared<GraphicsObject>(std::move(vao), std::move(ebo))).first->second;

			LOG_INFO("Success load obj file: {0}", OBJrelativePath);
			return newOBJ;
		}
	}
	file.close();
	return nullptr;
}
bool ResourceManager::start_with(std::string& line, const char* text)
{
	size_t texLen = strlen(text);
	if (line.size() < texLen) return false;
	for (size_t i = 0; i < texLen; i++)
	{
		if (line[i] == text[i]) continue;
		else return false;
	}
	return true;
}
std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath, std::ios::in | std::ios::binary);
	if (!f.is_open()) 
	{
		LOG_ERROR("Failed to open file: {0}", relativeFilePath);
		f.close();
		return std::string();
	}
	std::stringstream buffer;
	buffer << f.rdbuf();
	f.close();
	return buffer.str();
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath,
	std::shared_ptr<RenderEngine::ShaderProgramLayout> layout)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty()) 
	{
		LOG_ERROR("Vertex shader file is empty");
		return nullptr;
	}
	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		LOG_ERROR("Fragment shader file is empty");
		return nullptr;
	}

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader = 
		m_ShaderPrograms.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString, std::move(layout))).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	LOG_ERROR("Can't load shader program: \nVertex: {0} \nFragment: {1}", vertexPath, fragmentPath);
	return nullptr;
}
std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_ShaderPrograms.find(shaderName);
	if (it != m_ShaderPrograms.end())
	{
		return it->second;
	}
	LOG_ERROR("Can't find shader program: {0}", shaderName);
	return nullptr;
}
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channels = 0;
	int width = 0;
	int height = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

	if (!pixels)
	{
		LOG_ERROR("Can't load image: {0}", texturePath);
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Texture2D> newTexture = m_textures.emplace(textureName,
		std::make_shared<RenderEngine::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);

	return newTexture;
}
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}
	LOG_ERROR("Can't find texture: {0}", textureName);
	return nullptr;
}
std::shared_ptr<RenderEngine::Material> ResourceManager::loadMaterial(const std::string& materialName, const std::string& textureName, const std::string& shaderName)
{
	auto shader = getShaderProgram(shaderName);
	auto texture = getTexture(textureName);

	if (shader == nullptr)
	{
		LOG_ERROR("Can't find shader program {0} for material {1}", shaderName, materialName);
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Material> newMaterial =
	m_materials.emplace(materialName, std::make_shared<RenderEngine::Material>(shader, texture)).first->second;

	return newMaterial;
}
std::shared_ptr<RenderEngine::Material> ResourceManager::getMaterial(const std::string& materialName)
{
	MaterialsMap::const_iterator it = m_materials.find(materialName);
	if (it != m_materials.end())
	{
		return it->second;
	}
	LOG_ERROR("Can't find material: {0}", materialName);
	return nullptr;
}
std::vector<std::string> ResourceManager::getNamesShadersProgram()
{
	std::vector<std::string> data;

	for (const auto& curShader : m_ShaderPrograms)
	{
		data.push_back(curShader.first);
	}

	return data;
}
std::vector<std::string> ResourceManager::getNamesTextures2D()
{
	std::vector<std::string> data;

	for (const auto& curTexture : m_textures)
	{
		data.push_back(curTexture.first);
	}

	return data;
}
std::vector<std::string> ResourceManager::getNamesMaterials()
{
	std::vector<std::string> data;

	for (const auto& curMaterial : m_materials)
	{
		data.push_back(curMaterial.first);
	}

	return data;
}
std::vector<std::string> ResourceManager::getNamesObjs()
{
	std::vector<std::string> data;

	for (const auto& curObj : m_obj_files)
	{
		data.push_back(curObj.first);
	}

	return data;
}
std::vector<std::string> ResourceManager::getNamesFilesInDirectory(std::string relativeFilePath)
{
	std::vector<std::string> data;
	std::filesystem::path path;

	std::string tempStr;
	std::string temp2Str;
	std::string param = m_path + "/";

	for (auto& p : std::filesystem::directory_iterator(m_path + "/" + relativeFilePath))
	{
		path = p;
		tempStr = path.std::filesystem::path::generic_string();

		temp2Str = tempStr.substr(param.size());

		data.push_back(temp2Str);
	}
	return data;
}
std::string ResourceManager::getNameShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram> pShader)
{
	std::string outStr = "";
	for (const auto& curShader : m_ShaderPrograms)
	{
		if (curShader.second == pShader)
		{
			outStr = curShader.first;
			break;
		}
	}
	return outStr;
}
std::string ResourceManager::getNameTexture2D(std::shared_ptr<RenderEngine::Texture2D> pTexture)
{
	std::string outStr = "";
	for (const auto& curTexture : m_textures)
	{
		if (curTexture.second == pTexture)
		{
			outStr = curTexture.first;
			break;
		}
	}
	return outStr;
}
//std::shared_ptr<RenderEngine::SpriteRenderer>  ResourceManager::loadSpriteRenderer(const std::string& SpriteRendererName, const std::string& textureName,
//	const std::string& shaderName, const std::string& subTextureName)
//{
//	auto pTexture = getTexture(textureName);
//	if (!pTexture)
//	{
//		LOG_ERROR("Can't find texture: {0} for the SpriteRenderer: {1}", textureName, SpriteRendererName);
//		return nullptr;
//	}
//
//	auto pShaderProgram = getShaderProgram(shaderName);
//	if (!pShaderProgram)
//	{
//		LOG_ERROR("Can't find shader program: {0} for the SpriteRenderer: {1}", shaderName, SpriteRendererName);
//		return nullptr;
//	}
//	std::shared_ptr<RenderEngine::SpriteRenderer>& newSpriteRenderer = m_SpriteRenderers.emplace(SpriteRendererName, std::make_shared<RenderEngine::SpriteRenderer>(pTexture, subTextureName, 
//		pShaderProgram)).first->second;
//	return newSpriteRenderer;
//}
//std::shared_ptr<RenderEngine::SpriteRenderer> ResourceManager::getSpriteRenderer(const std::string& SpriteRendererName)
//{
//	SpriteRenderersMap::const_iterator it = m_SpriteRenderers.find(SpriteRendererName);
//	if (it != m_SpriteRenderers.end())
//	{
//		return it->second;
//	}
//	LOG_ERROR("Can't find SpriteRenderer: {0}", SpriteRendererName);
//	return nullptr;
//}
//std::shared_ptr<RenderEngine::GraphicsObject> ResourceManager::loadGraphicsObject(const std::string& objName, const std::string& shaderName, const std::string& source)
//{
//	auto pShaderProgram = getShaderProgram(shaderName);
//	if (!pShaderProgram)
//	{
//		LOG_ERROR("Can't find shader program: {0} for the grapics object: {1}", shaderName, objName);
//		return nullptr;
//	}
//
//	std::shared_ptr<RenderEngine::GraphicsObject>& graphObj = m_graph_objs.emplace(objName, std::make_shared<RenderEngine::GraphicsObject>
//		(pShaderProgram, m_path + "/" + source)).first->second;
//
//	return graphObj;
//}
//std::shared_ptr<RenderEngine::GraphicsObject> ResourceManager::getGraphicsObject(const std::string& objName)
//{
//	GraphObjMap::const_iterator it = m_graph_objs.find(objName);
//	if (it != m_graph_objs.end())
//	{
//		return it->second;
//	}
//	LOG_ERROR("Can't find graphics object: {0}", objName);
//	return nullptr;
//}
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(
	std::string textureName,
	std::vector<std::string> subTextures,
	std::string texturePath,
	const unsigned int width,
	const unsigned int height,
	const unsigned int subTextureWidth,
	const unsigned int subTextureHeight)
{
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));
	if (pTexture) 
	{
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = height;
		for (auto& currentSubTextureName : subTextures)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / width, static_cast<float>(currentTextureOffsetY - subTextureHeight) / height);
			glm::vec2 rightTop(static_cast<float>(currentTextureOffsetX + subTextureWidth) / width, static_cast<float>(currentTextureOffsetY) / height);
			pTexture->addSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTop);
			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= width)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
		}
	}
	return pTexture;
}

//std::shared_ptr<RenderEngine::Animator>  ResourceManager::loadAnimator(
//	const std::string& animatorName,
//	const std::string& SpriteRendererName)
//{
//	std::shared_ptr<RenderEngine::Animator>& newAnimator = m_animators.emplace(animatorName, std::make_shared<RenderEngine::Animator>(SpriteRendererName)).first->second;
//	return newAnimator;
//}
//std::shared_ptr<RenderEngine::Animator> ResourceManager::getAnimator(const std::string& animatorName)
//{
//	AnimatorsMap::const_iterator it = m_animators.find(animatorName);
//	if (it != m_animators.end())
//	{
//		return it->second;
//	}
//	std::cerr << "Can't find animator: " << animatorName << "\n";
//	return nullptr;
//}