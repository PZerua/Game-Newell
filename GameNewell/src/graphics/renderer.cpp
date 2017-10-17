/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "renderer.h"

#include <src/math/math.h>

namespace gfx
{

Renderer::Renderer()
{
	m_vao = std::make_unique<VertexArray>();

	// -- Setup indices --
	GLubyte indices[GN_QUAD_INDICES_SIZE] = { 0, 1, 2, 1, 3, 2 };
	m_ebo = std::make_unique<IndexBuffer>(indices, (GLsizei)(GN_QUAD_INDICES_SIZE * sizeof(GLubyte)));

	// -- Setup vertex data --
	GLfloat vertexData[GN_QUAD_VERTEXUV_SIZE] =
	{
		// Pos		// Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	// Vbo for the indexed quad
	m_vbo = std::make_unique<VertexBuffer>(vertexData, (GLsizei)(GN_QUAD_VERTEXUV_SIZE * sizeof(GLfloat)));

	unsigned stride = 4; // (2 + 2) Vertex + uv

	// Vertex data
	glEnableVertexAttribArray(ATTRIBUTE_VERTEX);
	glVertexAttribPointer(ATTRIBUTE_VERTEX, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)0);

	// Uv data
	glEnableVertexAttribArray(ATTRIBUTE_UV);
	glVertexAttribPointer(ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	// -- Setup a vbo to store the model matrix of each renderable --
	m_vbo_modelMatrices = std::make_unique<VertexBuffer>();

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

	// -- Setup texture indices to fetch the correct texture from the texture array in the shader --
	m_vbo_textureIndices = std::make_unique<VertexBuffer>();

	// I use a separate VBO for texture indices
	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_TEXTUREINDICES);
	glVertexAttribPointer(ATTRIBUTE_INSTANCE_TEXTUREINDICES, 1, GL_FLOAT, GL_FALSE, sizeof(GLuint), (void*)0);

	// One texture index for each instance
	glVertexAttribDivisor(ATTRIBUTE_INSTANCE_TEXTUREINDICES, 1);

	m_vao->unbind();
}

void Renderer::addRenderable(Sprite *sprite)
{
	m_transformations.push_back(sprite->getModel());
	m_textureIndices.push_back(sprite->getTextureIndex());

	m_vbo_modelMatrices->changeData(&m_transformations[0], (GLsizei)(m_transformations.size() * sizeof(math::mat4)), GL_STREAM_DRAW);
	m_vbo_textureIndices->changeData(&m_textureIndices[0], (GLsizei)(m_transformations.size() * sizeof(GLuint)), GL_STREAM_DRAW);
}

void Renderer::render()
{
	m_vao->bind();

	glDrawElementsInstanced(GL_TRIANGLES, GN_QUAD_INDICES_SIZE, GL_UNSIGNED_BYTE, NULL, (GLsizei) m_transformations.size());

	m_vao->unbind();

	m_transformations.clear();
	m_textureIndices.clear();
}

}