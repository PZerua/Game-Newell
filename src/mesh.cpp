/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "mesh.h"
#include <cassert>
#include "shader.h"

CMesh::~CMesh()
{
	if (m_vertices_vbo_id) glDeleteBuffersARB(1, &m_vertices_vbo_id);
	if (m_uvs_vbo_id) glDeleteBuffersARB(1, &m_uvs_vbo_id);

	clear();
}

void CMesh::clear()
{
	m_vertices.clear();
	m_uvs.clear();
}

void CMesh::render(int primitive, CShader* shader)
{
	assert(m_vertices.size() && "No vertices in this mesh");

	int vertex_location = shader->getAttribLocation("a_vertex");
	assert(vertex_location != -1 && "No a_vertex found in shader");

	if (vertex_location == -1)
		return;

	glEnableVertexAttribArray(vertex_location);
	if (m_vertices_vbo_id)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertices_vbo_id);
		glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}
	else
		glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, &m_vertices[0]);

	int uv_location = -1;
	if (m_uvs.size())
	{
		uv_location = shader->getAttribLocation("a_uv");
		if (uv_location != -1)
		{
			glEnableVertexAttribArray(uv_location);
			if (m_uvs_vbo_id)
			{
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_uvs_vbo_id);
				glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			}
			else
				glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, &m_uvs[0]);
		}
	}

	glDrawArrays(primitive, 0, (GLsizei)m_vertices.size());

	glDisableVertexAttribArray(vertex_location);
	if (uv_location != -1) glDisableVertexAttribArray(uv_location);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void CMesh::uploadToVRAM()
{
	//delete old
	if (m_vertices_vbo_id) glDeleteBuffersARB(1, &m_vertices_vbo_id);
	if (m_uvs_vbo_id) glDeleteBuffersARB(1, &m_uvs_vbo_id);

	glGenBuffersARB(1, &m_vertices_vbo_id); //generate one handler (id)
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertices_vbo_id); //bind the handler
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_vertices.size() * 3 * sizeof(float), &m_vertices[0], GL_STATIC_DRAW_ARB); //upload data

	if (m_uvs.size())
	{
		glGenBuffersARB(1, &m_uvs_vbo_id); //generate one handler (id)
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_uvs_vbo_id); //bind the handler
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_uvs.size() * 2 * sizeof(float), &m_uvs[0], GL_STATIC_DRAW_ARB); //upload data
	}
}

void CMesh::createQuad(float center_x, float center_y, float w, float h, int row, int col)
{
	m_vertices.clear();
	m_uvs.clear();

	// Create six vertices (3 for upper left triangle and 3 for lower right)
	m_vertices.push_back(glm::vec3(center_x + w*0.5f, center_y + h*0.5f, 0.0f));
	m_vertices.push_back(glm::vec3(center_x - w*0.5f, center_y - h*0.5f, 0.0f));
	m_vertices.push_back(glm::vec3(center_x + w*0.5f, center_y - h*0.5f, 0.0f));
	m_vertices.push_back(glm::vec3(center_x - w*0.5f, center_y + h*0.5f, 0.0f));
	m_vertices.push_back(glm::vec3(center_x - w*0.5f, center_y - h*0.5f, 0.0f));
	m_vertices.push_back(glm::vec3(center_x + w*0.5f, center_y + h*0.5f, 0.0f));

	// Texture coordinates (supposing always tilemap of 8 * 8 tiles) -> 1 / 8 = 0.125
	m_uvs.push_back(glm::vec2(0.125f + 0.125f * col, 0.0f + 0.125f * row));
	m_uvs.push_back(glm::vec2(0.0f + 0.125f * col, 0.125f + 0.125f * row));
	m_uvs.push_back(glm::vec2(0.125f + 0.125f * col, 0.125f + 0.125f * row));
	m_uvs.push_back(glm::vec2(0.0f + 0.125f * col, 0.0f + 0.125f * row));
	m_uvs.push_back(glm::vec2(0.0f + 0.125f * col, 0.125f + 0.125f * row));
	m_uvs.push_back(glm::vec2(0.125f + 0.125f * col, 0.0f + 0.125f * row));
}

void CMesh::setUVs(int row, int col)
{
	m_uvs.clear();

	m_uvs.push_back(glm::vec2(0.125f + 0.125f * col, 0.0f + 0.125f * row));
	m_uvs.push_back(glm::vec2(0.0f + 0.125f * col, 0.125f + 0.125f * row));
	m_uvs.push_back(glm::vec2(0.125f + 0.125f * col, 0.125f + 0.125f * row));
	m_uvs.push_back(glm::vec2(0.0f + 0.125f * col, 0.0f + 0.125f * row));
	m_uvs.push_back(glm::vec2(0.0f + 0.125f * col, 0.125f + 0.125f * row));
	m_uvs.push_back(glm::vec2(0.125f + 0.125f * col, 0.0f + 0.125f * row));
}