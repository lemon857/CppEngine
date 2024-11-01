#pragma once

#include <memory>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace RenderEngine 
{
	class VertexArray;
	class VertexBuffer;
	class Material;

	class Line 
	{
	public:
		Line(Material* pMaterial, float size = 1.f);
		~Line();

		inline Line(const Line&) = delete;
		inline Line& operator=(const Line&) = delete;

		void render(const glm::vec3 position, const glm::vec3 dir, glm::vec3 color) const;
		void render_from_to(const glm::vec3 position, const glm::vec3 to, glm::vec3 color) const;

		void render(const glm::vec3 position, const glm::vec3 dir, glm::vec4 color) const;
		void render_from_to(const glm::vec3 position, const glm::vec3 to, glm::vec4 color) const;

		void set_prj_mat(glm::mat4 prj);

		void set_size(float size);

		inline Material* get_material() { return m_pMaterial; }
	private:
		Material* m_pMaterial;
		float m_size;

		VertexArray* m_vertexArray;
		VertexBuffer* m_vertexCoordsBuffer;
	};
}
