#include "EngineCore/Components/SpriteRenderer.h"

#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/IGameObject.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer()
	: IComponent()
{

}

SpriteRenderer::~SpriteRenderer()
{
	delete m_indexBuffer;
	delete m_textureCoordsBuffer;
	delete m_vertexCoordsBuffer;
	delete m_vertexArray.get();
}

void SpriteRenderer::init(std::shared_ptr<RenderEngine::Texture2D> pTexture,
	std::string initialSubTexture,
	std::shared_ptr<RenderEngine::ShaderProgram> pShaderProgram)
{

	m_pShaderProgram = std::move(pShaderProgram);
	m_pTextureAtlas = std::move(pTexture);

	m_vertexArray = std::make_shared<RenderEngine::VertexArray>();
	m_vertexCoordsBuffer = new RenderEngine::VertexBuffer();
	m_textureCoordsBuffer = new RenderEngine::VertexBuffer();
	m_indexBuffer = new RenderEngine::IndexBuffer();

	const GLfloat vertexCoords[] = {
		//2--3  1
		//| / /	|
		//1	 3--2	
		//1--2
		//| /|
		//0--3
		//X --- Y
		-0.5f,-0.5f, 0.f,
		-0.5f, 0.5f, 0.f,
		 0.5f, 0.5f, 0.f,
		 0.5f, -0.5f, 0.f
	};

	auto aSubTexture = m_pTextureAtlas->getSubTexture(initialSubTexture);
	const GLfloat textureCoords[] = {
		//U --- V
		aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
		aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
		aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
		aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
	};
	const GLuint indexes[] = { 0, 1, 2, 2, 3, 0 };

	m_vertexCoordsBuffer->init(&vertexCoords, 3 * 4 * sizeof(GLfloat));
	RenderEngine::VertexBufferLayout vertexCoordsLayout;
	vertexCoordsLayout.addElementLayoutFloat(3, false);
	m_vertexArray->addBuffer(*m_vertexCoordsBuffer, vertexCoordsLayout);

	m_textureCoordsBuffer->init(&textureCoords, 2 * 4 * sizeof(GLfloat));
	RenderEngine::VertexBufferLayout textureCoordsLayout;
	textureCoordsLayout.addElementLayoutFloat(2, false);
	m_vertexArray->addBuffer(*m_textureCoordsBuffer, textureCoordsLayout);

	m_indexBuffer->init(&indexes, 6);

	m_vertexArray->unbind();
	m_indexBuffer->unbind();
}

void SpriteRenderer::setSubTexture(std::string subTexture)
{
	auto aSubTexture = m_pTextureAtlas->getSubTexture(subTexture);
	const GLfloat textureCoords[] = {
		//U --- V
		aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
		aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
		aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
		aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
	}; 
	m_textureCoordsBuffer->update(&textureCoords, 2 * 4 * sizeof(GLfloat));
}

void SpriteRenderer::update(const double delta)
{
	Transform* transform = m_targetObj->getComponent<Transform>();
	if (transform == nullptr) return;

	m_pShaderProgram->use();

	glm::vec3 scale = transform->get_scale();
	glm::vec3 pos = transform->get_position();
	glm::vec3 rot = transform->get_rotation();

	glm::mat4 scaleMat(
		scale[0], 0, 0, 0,
		0, scale[1], 0, 0,
		0, 0, scale[2], 0,
		0, 0, 0, 1);

	glm::mat4 translateMat(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		pos[0], pos[1], pos[2], 1);

	float xRotRadians = glm::radians(rot[0]);
	float yRotRadians = glm::radians(rot[1]);
	float zRotRadians = glm::radians(rot[2]);

	glm::mat4 rotateXmat(
		1, 0, 0, 0,
		0, cos(xRotRadians), -sin(xRotRadians), 0,
		0, sin(xRotRadians), cos(xRotRadians), 0,
		0, 0, 0, 1);

	glm::mat4 rotateYmat(
		cos(yRotRadians), 0, -sin(yRotRadians), 0,
		0, 1, 0, 0,
		sin(yRotRadians), 0, cos(yRotRadians), 0,
		0, 0, 0, 1);

	glm::mat4 rotateZmat(
		cos(zRotRadians), -sin(zRotRadians), 0, 0,
		sin(zRotRadians), cos(zRotRadians), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	glm::mat4 model = translateMat * rotateXmat * rotateYmat * rotateZmat * scaleMat;

	m_pShaderProgram->setMatrix4("modelMat", model);

	RenderEngine::Renderer::bindTexture(*m_pTextureAtlas);
	RenderEngine::Renderer::drawTriangles(*m_vertexArray, *m_indexBuffer);
}