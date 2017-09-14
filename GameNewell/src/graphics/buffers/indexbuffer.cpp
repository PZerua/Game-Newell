/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "indexbuffer.h"

namespace gfx
{

	IndexBuffer::IndexBuffer(GLushort *data, GLuint size) :
		m_size(size)
	{
		glGenBuffers(1, &m_eboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId);
		glBufferData(m_eboId, size * sizeof(GLushort), data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_eboId);
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId);
	}

	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

} // namespace gfx