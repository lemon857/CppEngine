#include "EngineCore/System/Loaders.h"

#include "EngineCore/Renderer3D/GraphicsObject.h"

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"

#include "EngineCore/System/SysFunc.h"

#include <cstdio>

#include <fstream>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION 
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#include <stb_image.h>
#include <stb_image_write.h>

void INIdata::add_region(std::string name, BaseINIregion* region)
{
	regions.emplace(name, std::move(region));
}

BaseINIregion* INIdata::get_region(std::string name)
{
	std::map<std::string, BaseINIregion*>::const_iterator it = regions.find(name);
	if (it != regions.end())
	{
		return it->second;
	}
	return nullptr;
}

namespace loaders
{
	GraphicsObject* load_obj_file(const std::string& OBJPath)
	{
		std::ifstream file;
		file.open(OBJPath);
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
				if (sysfunc::start_with(line, "v "))
				{
					GLfloat x, y, z;
					sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
					temp_pos.push_back(glm::vec3(x, y, z));
					if (!need_normalize_vertex_pos && (x > 1.f || x < -1.f)) need_normalize_vertex_pos = true;
#ifdef DEBUG_CONSOLE_OBJ_LOAD
					LOG_INFO("Vert: {0}x{1}x{2}", x, y, z);
#endif // DEBUG_CONSOLE_OBJ_LOAD

				}
				else if (sysfunc::start_with(line, "vn "))
				{
					GLfloat x, y, z;
					sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
					temp_norms.push_back(glm::vec3(x, y, z));
#ifdef DEBUG_CONSOLE_OBJ_LOAD
					LOG_INFO("Norms: {0}x{1}x{2}", x, y, z);
#endif // DEBUG_CONSOLE_OBJ_LOAD
				}
				else if (sysfunc::start_with(line, "vt "))
				{
					GLfloat x, y;
					sscanf(line.c_str(), "vt %f %f", &x, &y);
					temp_texs.push_back(glm::vec2(x, y));
#ifdef DEBUG_CONSOLE_OBJ_LOAD
					LOG_INFO("Texs: {0}x{1}", x, y);
#endif // DEBUG_CONSOLE_OBJ_LOAD
				}
				// WARNING region (# faces) in .obj file must be lastest
				else if (sysfunc::start_with(line, "f "))
				{
					int indexX, textureX, normalX;
					int indexY, textureY, normalY;
					int indexZ, textureZ, normalZ;
					sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
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

			if (!ebo->init(index_array.data(), index_array.size() * sizeof(GLuint)))
			{
				file.close();
				return nullptr;
			}

			vao->unbind();
			ebo->unbind();

			file.close();

			return new GraphicsObject(vao, ebo);
		}
		file.close();
		return nullptr;
	}

	ErrorCode load_ini(const std::string& INIpath, INIdata& data, const bool isWrite)
	{
		std::ifstream f;
		f.open(INIpath, std::ios::in);

		if (f.is_open() && !isWrite)
		{
			std::string line;
			std::string region = "";
			while (std::getline(f, line))
			{
				if (line.find('#') != -1)
				{
					if (line.find('#') == 0) continue;
					line = line.substr(0, line.find('#'));
				}
				if (line[0] == '[')
				{
					region = line.substr(1, line.length() - 2);
				}
				else if (data.get_region(region) != nullptr)
				{
					std::string name = line.substr(0, line.find('='));
					std::string value = line.substr(line.find('=') + 1);
					data.get_region(region)->parse(name, value);
				}
			}
			return ErrorCode::null;
		}
		std::ofstream writeStream;
		writeStream.open(INIpath);

		if (writeStream.is_open())
		{
			for (auto& i : data.regions)
			{
				writeStream << '[' << i.first << ']' << '\n';
				writeStream << i.second->get_str_data();
			}
			return ErrorCode::null;
		}
		else
		{
			writeStream.close();
			return ErrorCode::errorFileRead;
		}
	}

	unsigned char* load_image(const char* fullpath, int* width, int* height, int* channels, bool flip)
	{
		stbi_set_flip_vertically_on_load(flip);
		return stbi_load(fullpath, width, height, channels, 0);
	}

	int save_image_png(unsigned char* pixels, const char* fullpath, int width, int height, int channels)
	{
		stbi_flip_vertically_on_write(true);
		return stbi_write_png(fullpath, width, height, channels, pixels, width * channels);
	}

	void clear_image(unsigned char* image_data)
	{
		stbi_image_free(image_data);
	}
}
