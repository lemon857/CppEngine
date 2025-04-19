#pragma once

#include <memory>

namespace RenderEngine
{
	class VertexArray;
	class IndexBuffer;
}

struct GraphicsObject
{
	GraphicsObject()
		: vertex_array(nullptr)
		, index_buffer(nullptr)
	{
	}
	GraphicsObject(std::shared_ptr<RenderEngine::VertexArray> vao,
	std::shared_ptr<RenderEngine::IndexBuffer> ebo)
		: vertex_array(std::move(vao))
		, index_buffer(std::move(ebo))
	{
	}
	std::shared_ptr<RenderEngine::VertexArray> vertex_array;
	std::shared_ptr<RenderEngine::IndexBuffer> index_buffer;
};