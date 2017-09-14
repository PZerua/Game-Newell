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
		GLuint m_componentCount;

	public:

		VertexBuffer(GLfloat *data, GLsizei size, GLuint componentCount);
		~VertexBuffer();

		void bind() const;
		void unbind() const;

		inline GLuint getComponentCount() { return m_componentCount; }
	};

} // namespace gfx