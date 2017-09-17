/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "vertexarray.h"

namespace gfx
{

	VertexArray::VertexArray() :
		m_indexCount(0)
	{
		glGenVertexArrays(1, &m_vaoId);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_vaoId);
	}

	void VertexArray::addVertexBuffer(VertexBuffer* vertexBuffer, unsigned char flag)
	{
		unsigned stride = 0;

		if (flag & BUFFER_COLOR)
			stride += 5;

		if (flag & BUFFER_UV)
			stride += 2;

		bind();

		vertexBuffer->bind();

		glEnableVertexAttribArray(m_indexCount);
		glVertexAttribPointer(m_indexCount++, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), NULL);

		if (flag & BUFFER_COLOR)
		{
			glEnableVertexAttribArray(m_indexCount);
			glVertexAttribPointer(m_indexCount++, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(2 * sizeof(float)));
		}

		if (flag & BUFFER_UV)
		{
			glEnableVertexAttribArray(m_indexCount);
			glVertexAttribPointer(m_indexCount++, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(6 * sizeof(float)));
		}

		unbind();
	}

	void VertexArray::addIndexBuffer(IndexBuffer* indexBuffer)
	{
		bind();
		indexBuffer->bind();
		unbind();
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_vaoId);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

} // namespace gfx