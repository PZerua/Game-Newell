/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "mesh.h"
#include <cassert>
#include "shader.h"

CMesh::~CMesh()
{
	if (m_vertices_vbo_id) glDeleteBuffers(1, &m_vertices_vbo_id);
	if (m_uvs_vbo_id) glDeleteBuffers(1, &m_uvs_vbo_id);

	clear();
}

void CMesh::clear()
{
	m_vertices.clear();
	m_uvs.clear();
}

void CMesh::render(int primitive, CShader* shader)
{
	// Setup vertices
	int vertex_location = shader->getAttribLocation("a_vertex");

	glEnableVertexAttribArray(vertex_location);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertices_vbo_id);
	glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Setup uvs
	int uv_location = shader->getAttribLocation("a_uv");

	if (m_uvs.size())
	{
		glEnableVertexAttribArray(uv_location);
		glBindBuffer(GL_ARRAY_BUFFER, m_uvs_vbo_id);
		glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	// Draw the quads
	glDrawArrays(primitive, 0, (GLsizei)m_vertices.size());

	// Disable bindings
	glDisableVertexAttribArray(vertex_location);
	if (m_uvs.size()) glDisableVertexAttribArray(uv_location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CMesh::uploadToVRAM()
{
	// Delete old VBOs
	if (m_vertices_vbo_id) glDeleteBuffers(1, &m_vertices_vbo_id);
	if (m_uvs_vbo_id) glDeleteBuffers(1, &m_uvs_vbo_id);

	glGenBuffers(1, &m_vertices_vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertices_vbo_id);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 3 * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);

	if (m_uvs.size())
	{
		glGenBuffers(1, &m_uvs_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_uvs_vbo_id);
		glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * 2 * sizeof(float), &m_uvs[0], GL_STATIC_DRAW);
	}
}

void CMesh::createQuad(float center_x, float center_y, float w, float h, int row, int col)
{
	m_vertices.clear();

	// Create four vertices for the quad (then using TRIANGLE_STRIP)
	m_vertices.push_back(glm::vec3(center_x - w*0.5f, center_y - h*0.5f, 0.0f));
	m_vertices.push_back(glm::vec3(center_x + w*0.5f, center_y - h*0.5f, 0.0f));
	m_vertices.push_back(glm::vec3(center_x - w*0.5f, center_y + h*0.5f, 0.0f));
	m_vertices.push_back(glm::vec3(center_x + w*0.5f, center_y + h*0.5f, 0.0f));

	// Texture coordinates
	setUVs(row, col);

	uploadToVRAM();
}

void CMesh::setUVs(int row, int col)
{
	m_uvs.clear();

	// The uvs of a tile depend on the number of tiles in a Tilemap
	float uv_offset = 1.0f / TILEMAP_SIZE;

	// Texture coordinates
	m_uvs.push_back(glm::vec2(0.0f + uv_offset * col, 0.0f + uv_offset * row));
	m_uvs.push_back(glm::vec2(uv_offset + uv_offset * col, 0.0f + uv_offset * row));
	m_uvs.push_back(glm::vec2(0.0f + uv_offset * col, uv_offset + uv_offset * row));
	m_uvs.push_back(glm::vec2(uv_offset + uv_offset * col, uv_offset + uv_offset * row));
}