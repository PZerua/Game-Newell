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

	// Vertex, color and uv data is set in a single vbo, but they are optional
	unsigned stride = 0;
	unsigned offset = 0;

	if (flag & VBO_BUFFER_VERTEX)
		stride += 2;

	if (flag & VBO_BUFFER_COLOR)
		stride += 3;

	if (flag & VBO_BUFFER_UV)
		stride += 2;

	if (flag & VBO_BUFFER_VERTEX)
	{
		glEnableVertexAttribArray(ATTRIBUTE_VERTEX);
		glVertexAttribPointer(ATTRIBUTE_VERTEX, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)offset);
		offset += 2;
	}

	if (flag & VBO_BUFFER_COLOR)
	{
		glEnableVertexAttribArray(ATTRIBUTE_COLOR);
		glVertexAttribPointer(ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
		offset += 3;
	}

	if (flag & VBO_BUFFER_UV)
	{
		glEnableVertexAttribArray(ATTRIBUTE_UV);
		glVertexAttribPointer(ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
	}

	unbind();
}

void VertexArray::addVertexBufferInstancedModel(VertexBuffer* vertexBuffer)
{
	bind();
	vertexBuffer->bind();

	// I use a separate VBO for instanced arrays
	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY + 0);
	glVertexAttribPointer(ATTRIBUTE_INSTANCE_ARRAY + 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(math::vec4), (void*)0);
	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY + 1);
	glVertexAttribPointer(ATTRIBUTE_INSTANCE_ARRAY + 1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(math::vec4), (void*)(1 * sizeof(math::vec4)));
	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY + 2);
	glVertexAttribPointer(ATTRIBUTE_INSTANCE_ARRAY + 2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(math::vec4), (void*)(2 * sizeof(math::vec4)));
	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY + 3);
	glVertexAttribPointer(ATTRIBUTE_INSTANCE_ARRAY + 3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(math::vec4), (void*)(3 * sizeof(math::vec4)));

	// For each vec4 of the instance array (mat4 divided in four parts)
	glVertexAttribDivisor(ATTRIBUTE_INSTANCE_ARRAY + 0, 1);
	glVertexAttribDivisor(ATTRIBUTE_INSTANCE_ARRAY + 1, 1);
	glVertexAttribDivisor(ATTRIBUTE_INSTANCE_ARRAY + 2, 1);
	glVertexAttribDivisor(ATTRIBUTE_INSTANCE_ARRAY + 3, 1);

	vertexBuffer->unbind();
	unbind();

	return;
}

void VertexArray::addVertexBufferInstancedTextureIndices(VertexBuffer* vertexBuffer)
{
	bind();
	vertexBuffer->bind();

	// I use a separate VBO for texture indices
	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_TEXTUREINDICES);
	glVertexAttribPointer(ATTRIBUTE_INSTANCE_TEXTUREINDICES, 1, GL_FLOAT, GL_FALSE, sizeof(GLuint), (void*)0);

	// One texture index for each instance
	glVertexAttribDivisor(ATTRIBUTE_INSTANCE_TEXTUREINDICES, 1);

	vertexBuffer->unbind();
	unbind();

	return;
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