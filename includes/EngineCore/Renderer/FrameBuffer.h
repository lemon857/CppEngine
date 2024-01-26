#pragma once

typedef unsigned int GLuint;

//#include <glad/glad.h>
namespace RenderEngine
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&& FrameBuffer) noexcept;
		FrameBuffer& operator=(const FrameBuffer&) = delete;
		FrameBuffer& operator=(FrameBuffer&& FrameBuffer) noexcept;

		bool init(int window_width, int window_height);
		void bind();
		void unbind();

		void bind_texture();
		void unbind_texture();

	private:
		GLuint m_ID;
		GLuint m_ID_tex;
		GLuint m_ID_rbo;
	};
}