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
	initVbo(VBO_BUFFER_VERTEX | VBO_INSTANCED); // Vertex buffer object (vertex, color, uv) and transformations
	initEbo(); // Element buffer object (indices)
	initVao(); // Vertex array object
}

void Renderer::render()
{
	m_vao->bind();

	glDrawElementsInstanced(GL_TRIANGLES, GN_QUAD_INDICES_SIZE, GL_UNSIGNED_BYTE, NULL, (GLsizei) m_transformations.size());

	m_vao->unbind();

	m_transformations.clear();
}

void Renderer::addRenderable(Renderable2D *renderable)
{
	math::mat4 model(1.0f);

	model.translate(math::vec3(renderable->getPosition(), 0.0f));
	model.scale(math::vec3(renderable->getSize(), 1.0f));

	m_transformations.push_back(model);

	m_vbo_instanced->changeData(&m_transformations[0], m_transformations.size() * sizeof(math::mat4), GL_DYNAMIC_DRAW);
}

void Renderer::initVao()
{
	m_vao = std::make_unique<VertexArray>();
	m_vao->addVertexBuffer(m_vbo.get(), VBO_BUFFER_VERTEX);
	if (m_vbo_instanced)
		m_vao->addVertexBuffer(m_vbo_instanced.get(), VBO_INSTANCED);
	m_vao->addIndexBuffer(m_ebo.get());
}

void Renderer::initEbo()
{
	GLubyte indices[GN_QUAD_INDICES_SIZE] = { 0, 1, 2, 1, 3, 2 };
	m_ebo = std::make_unique<IndexBuffer>(indices, GN_QUAD_INDICES_SIZE * sizeof(GLubyte));
}

void Renderer::initVbo(unsigned char flag)
{
	if (flag & VBO_BUFFER_VERTEX)
	{
		GLfloat vertexData[GN_QUAD_VERTEX_SIZE] =
		{
			0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
		};

		m_vbo = std::make_unique<VertexBuffer>(vertexData, GN_QUAD_VERTEX_SIZE * sizeof(GLfloat));
	}

	if (flag & VBO_INSTANCED)
	{
		m_vbo_instanced = std::make_unique<VertexBuffer>();
	}

}

}