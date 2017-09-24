/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "vertexarray.h"

#include <src/math/vec4.h>

namespace gfx
{

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_vaoId);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_vaoId);
	}

	void VertexArray::addVertexBuffer(VertexBuffer* vertexBuffer, unsigned char flag)
	{
		bind();

		vertexBuffer->bind();

		// I use a separate VBO for instanced arrays, so when one is added just set the data and return
		if (flag & VBO_INSTANCED)
		{
			GLsizei vec4Size = sizeof(math::vec4);
			glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY);
			glVertexAttribPointer(ATTRIBUTE_INSTANCE_ARRAY, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
			glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY + 1);
			glVertexAttribPointer(ATTRIBUTE_INSTANCE_ARRAY + 1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
			glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY + 2);
			glVertexAttribPointer(ATTRIBUTE_INSTANCE_ARRAY + 2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
			glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY + 3);
			glVertexAttribPointer(ATTRIBUTE_INSTANCE_ARRAY + 3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

			glVertexAttribDivisor(ATTRIBUTE_INSTANCE_ARRAY, 1);
			glVertexAttribDivisor(ATTRIBUTE_INSTANCE_ARRAY + 1, 1);
			glVertexAttribDivisor(ATTRIBUTE_INSTANCE_ARRAY + 2, 1);
			glVertexAttribDivisor(ATTRIBUTE_INSTANCE_ARRAY + 3, 1);

			vertexBuffer->unbind();
			unbind();

			return;
		}

		// Vertex, color and uv data is set in a single vbo, but they are optional (vertex is optional because the vbo can only store and instanced array)
		unsigned stride = 0;

		if (flag & VBO_BUFFER_COLOR)
			stride += 5;

		if (flag & VBO_BUFFER_UV)
			stride += 2;

		if (flag & VBO_BUFFER_VERTEX)
		{
			glEnableVertexAttribArray(ATTRIBUTE_VERTEX);
			glVertexAttribPointer(ATTRIBUTE_VERTEX, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), NULL);
		}

		if (flag & VBO_BUFFER_COLOR)
		{
			glEnableVertexAttribArray(ATTRIBUTE_COLOR);
			glVertexAttribPointer(ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(2 * sizeof(float)));
		}

		if (flag & VBO_BUFFER_UV)
		{
			glEnableVertexAttribArray(ATTRIBUTE_UV);
			glVertexAttribPointer(ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(5 * sizeof(float)));
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