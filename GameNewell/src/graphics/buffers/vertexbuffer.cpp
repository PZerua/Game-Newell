/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "vertexbuffer.h"

namespace gfx
{

	VertexBuffer::VertexBuffer(GLfloat *data, GLsizei size, GLuint componentCount) :
		m_componentCount(componentCount)
	{
		glGenBuffers(1, &m_vboId);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
		glBufferData(m_vboId, size * sizeof(GLfloat), data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_vboId);
	}
	
	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

} // namespace gfx