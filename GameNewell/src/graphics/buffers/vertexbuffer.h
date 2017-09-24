/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <GL/glew.h>

namespace gfx
{

	class VertexBuffer
	{
	private:
		GLuint m_vboId;

	public:

		VertexBuffer(GLvoid *data, GLsizei size, GLenum drawType = GL_STATIC_DRAW);
		VertexBuffer();
		~VertexBuffer();

		void bind() const;
		void unbind() const;

		void changeData(GLvoid *data, GLsizei size, GLenum drawType = GL_STATIC_DRAW);
	};

} // namespace gfx