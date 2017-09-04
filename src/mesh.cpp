/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "mesh.h"
#include <cassert>
#include "shader.h"

CMesh::~CMesh()
{
	if (m_verticesVBO) glDeleteBuffers(1, &m_verticesVBO);
	if (m_uvsVBO) glDeleteBuffers(1, &m_uvsVBO);
	if (m_verticesVAO) glDeleteVertexArrays(1, &m_verticesVAO);

	clear();
}

void CMesh::clear()
{
	m_vertices.clear();
	m_verticesIndices.clear();
	m_uvs.clear();
}

void CMesh::render(int primitive, CShader* shader)
{
	// Bind VAO
	glBindVertexArray(m_verticesVAO);

	// Draw triangles
	glDrawElements(primitive, (GLsizei)m_verticesIndices.size(), GL_UNSIGNED_INT, 0);

	// Disable binding
	glBindVertexArray(0);
}

void CMesh::uploadToVRAM()
{
	// Delete old buffers and VAO
	if (m_verticesVAO) glDeleteVertexArrays(1, &m_verticesVAO);
	if (m_verticesVBO) glDeleteBuffers(1, &m_verticesVBO);
	if (m_uvsVBO) glDeleteBuffers(1, &m_uvsVBO);

	// Generate buffers and VAO
	glGenVertexArrays(1, &m_verticesVAO);
	glGenBuffers(1, &m_verticesVBO);
	glGenBuffers(1, &m_indicesEBO);
	glGenBuffers(1, &m_uvsVBO);

	// Bind the VAO
	glBindVertexArray(m_verticesVAO);

	// Setup vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);

	// Set vertices as vertex attribute to shader (pos 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	// Color attribute (pos 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Setup indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_verticesIndices.size() * sizeof(unsigned), &m_verticesIndices[0], GL_STATIC_DRAW);

	// Set uvs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void CMesh::addVertex(glm::vec3 pos, glm::vec2 uv)
{
	// Vertex
	m_vertices.push_back(pos.x);
	m_vertices.push_back(pos.y);
	m_vertices.push_back(pos.z);

	m_vertices.push_back(0.25f); // R
	m_vertices.push_back(0.35f); // G
	m_vertices.push_back(0.45f); // B

	m_vertices.push_back(uv.x);
	m_vertices.push_back(uv.y);

}

void CMesh::createQuad(float center_x, float center_y, float w, float h, int row, int col)
{
	m_vertices.clear();

	float uv_offset = 1.0f / TILEMAP_SIZE;

	// Create four vertices for the quad (then using TRIANGLE_STRIP)
	addVertex(glm::vec3(center_x - w*0.5f, center_y - h*0.5f, 0.0f), glm::vec2(0.0f + uv_offset * col, 0.0f + uv_offset * row));
	addVertex(glm::vec3(center_x + w*0.5f, center_y - h*0.5f, 0.0f), glm::vec2(uv_offset + uv_offset * col, 0.0f + uv_offset * row));
	addVertex(glm::vec3(center_x - w*0.5f, center_y + h*0.5f, 0.0f), glm::vec2(0.0f + uv_offset * col, uv_offset + uv_offset * row));
	addVertex(glm::vec3(center_x + w*0.5f, center_y + h*0.5f, 0.0f), glm::vec2(uv_offset + uv_offset * col, uv_offset + uv_offset * row));

	m_verticesIndices.push_back(0);
	m_verticesIndices.push_back(1);
	m_verticesIndices.push_back(2);

	m_verticesIndices.push_back(1);
	m_verticesIndices.push_back(3);
	m_verticesIndices.push_back(2);

	uploadToVRAM();
}
