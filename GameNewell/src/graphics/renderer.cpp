/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "renderer.h"

#include <src/math/math.h>
#include <src/utils/debugutils.h>

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

	// I use a separate VBO for instanced arrays
	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY + 0);
	glVertexAttribFormat(ATTRIBUTE_INSTANCE_ARRAY + 0, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(ATTRIBUTE_INSTANCE_ARRAY + 0, 3);

	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY + 1);
	glVertexAttribFormat(ATTRIBUTE_INSTANCE_ARRAY + 1, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(math::vec4));
	glVertexAttribBinding(ATTRIBUTE_INSTANCE_ARRAY + 1, 3);

	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY + 2);
	glVertexAttribFormat(ATTRIBUTE_INSTANCE_ARRAY + 2, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(math::vec4));
	glVertexAttribBinding(ATTRIBUTE_INSTANCE_ARRAY + 2, 3);

	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_ARRAY + 3);
	glVertexAttribFormat(ATTRIBUTE_INSTANCE_ARRAY + 3, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(math::vec4));
	glVertexAttribBinding(ATTRIBUTE_INSTANCE_ARRAY + 3, 3);

	glVertexBindingDivisor(3, 1);

	// I use a separate VBO for texture indices
	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_TEXTUREINDICES);
	glVertexAttribFormat(ATTRIBUTE_INSTANCE_TEXTUREINDICES, 1, GL_FLOAT, GL_FALSE, 0);

	// Bind to second vbo
	glVertexAttribBinding(ATTRIBUTE_INSTANCE_TEXTUREINDICES, 7);

	m_vao->unbind();
}

std::pair<GLuint, GLuint> Renderer::getTexture(const char *fileName)
{
	// Array texture id - Texture id inside the array - Is there a new texture array
	auto &tuple = m_manager.getTexture(fileName);

	// If new texture array is created
	if (std::get<2>(tuple))
		initRenderableGroup(std::get<0>(tuple));

	return {std::get<0>(tuple), std::get<1>(tuple)};
}

void Renderer::addRenderable(Sprite &sprite)
{
	auto &group = m_renderables[sprite.getTextureArrayId()];

	group.transformations.push_back(sprite.getModel());
	group.textureIndices.push_back(sprite.getTextureIndex());

	group.vbo_modelMatrices->changeData(&group.transformations[0], (GLsizei)(group.transformations.size() * sizeof(math::mat4)));
	group.vbo_textureIndices->changeData(&group.textureIndices[0], (GLsizei)(group.textureIndices.size() * sizeof(GLuint)));
}

void Renderer::initRenderableGroup(GLuint id)
{
	// -- Setup a vbo to store the model matrix of each renderable --
	m_renderables[id].vbo_modelMatrices = std::make_unique<VertexBuffer>();

	// -- Setup texture indices to fetch the correct texture from the texture array in the shader --
	m_renderables[id].vbo_textureIndices = std::make_unique<VertexBuffer>();
}

void Renderer::render()
{
	m_vao->bind();

	for (auto &elem : m_renderables)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, elem.first);
		glBindVertexBuffer(3, elem.second.vbo_modelMatrices->getId(), 0, 4 * sizeof(math::vec4));
		glBindVertexBuffer(7, elem.second.vbo_textureIndices->getId(), 0, sizeof(GLuint));
		glDrawElementsInstanced(GL_TRIANGLES, GN_QUAD_INDICES_SIZE, GL_UNSIGNED_BYTE, NULL, (GLsizei)elem.second.transformations.size());

		elem.second.transformations.clear();
		elem.second.textureIndices.clear();
	}

	m_vao->unbind();
}

}