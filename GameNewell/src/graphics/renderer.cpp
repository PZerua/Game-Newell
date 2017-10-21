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

	// The model matrices are uploaded as four vec4, the format is specified here and the vbo is bound later in the render pass
	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_MODELMATRIX + 0);
	glVertexAttribFormat(ATTRIBUTE_INSTANCE_MODELMATRIX + 0, 4, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_MODELMATRIX + 1);
	glVertexAttribFormat(ATTRIBUTE_INSTANCE_MODELMATRIX + 1, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(math::vec4));

	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_MODELMATRIX + 2);
	glVertexAttribFormat(ATTRIBUTE_INSTANCE_MODELMATRIX + 2, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(math::vec4));

	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_MODELMATRIX + 3);
	glVertexAttribFormat(ATTRIBUTE_INSTANCE_MODELMATRIX + 3, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(math::vec4));

	// Bint to third vbo (third position in shader attributes)
	glVertexAttribBinding(ATTRIBUTE_INSTANCE_MODELMATRIX + 0, ATTRIBUTE_INSTANCE_MODELMATRIX);
	glVertexAttribBinding(ATTRIBUTE_INSTANCE_MODELMATRIX + 1, ATTRIBUTE_INSTANCE_MODELMATRIX);
	glVertexAttribBinding(ATTRIBUTE_INSTANCE_MODELMATRIX + 2, ATTRIBUTE_INSTANCE_MODELMATRIX);
	glVertexAttribBinding(ATTRIBUTE_INSTANCE_MODELMATRIX + 3, ATTRIBUTE_INSTANCE_MODELMATRIX);

	// Get next attribute every instance (and not every vertex)
	glVertexBindingDivisor(ATTRIBUTE_INSTANCE_MODELMATRIX, 1);

	// The texture indices are uploaded as a GLuint, the format is specified here and the vbo is bound later in the render pass
	glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_TEXTUREINDICES);
	glVertexAttribFormat(ATTRIBUTE_INSTANCE_TEXTUREINDICES, 1, GL_FLOAT, GL_FALSE, 0);

	// Bint to seventh vbo (seventh position in shader attributes)
	glVertexAttribBinding(ATTRIBUTE_INSTANCE_TEXTUREINDICES, ATTRIBUTE_INSTANCE_TEXTUREINDICES);

	m_vao->unbind();
}

std::pair<GLuint, GLuint> Renderer::getTexture(const char *fileName)
{
	// Array texture id - Texture id inside the array - Is there a new texture array
	auto &textureInfo = m_manager.getTexture(fileName);

	// If new texture array is created
	if (std::get<2>(textureInfo))
		initRenderableGroup(std::get<0>(textureInfo));

	return {std::get<0>(textureInfo), std::get<1>(textureInfo)};
}

void Renderer::addRenderable(Sprite &sprite)
{
	auto &group = m_renderables[sprite.getTextureArrayId()];

	group.transformations.push_back(sprite.getModel());
	group.textureIndices.push_back(sprite.getTextureIndex());

	// TODO: I can optimize this with a render queue, now I'm changing the vbo every time a renderable is added
	group.vbo_modelMatrices->changeData(&group.transformations[0], (GLsizei)(group.transformations.size() * sizeof(math::mat4)));
	group.vbo_textureIndices->changeData(&group.textureIndices[0], (GLsizei)(group.textureIndices.size() * sizeof(GLuint)));
}

void Renderer::initRenderableGroup(GLuint id)
{
	// Setup the vbos to store the model matrices and texture indices of each group
	m_renderables[id].vbo_modelMatrices = std::make_unique<VertexBuffer>();
	m_renderables[id].vbo_textureIndices = std::make_unique<VertexBuffer>();
}

void Renderer::render()
{
	m_vao->bind();

	for (auto &elem : m_renderables)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, elem.first);
		glBindVertexBuffer(ATTRIBUTE_INSTANCE_MODELMATRIX, elem.second.vbo_modelMatrices->getId(), 0, 4 * sizeof(math::vec4));
		glBindVertexBuffer(ATTRIBUTE_INSTANCE_TEXTUREINDICES, elem.second.vbo_textureIndices->getId(), 0, sizeof(GLuint));
		glDrawElementsInstanced(GL_TRIANGLES, GN_QUAD_INDICES_SIZE, GL_UNSIGNED_BYTE, NULL, (GLsizei)elem.second.transformations.size());

		// Since the vectors depend on the current renderables to render, they need to be cleared for the next render pass
		elem.second.transformations.clear();
		elem.second.textureIndices.clear();
	}

	m_vao->unbind();
}

}
