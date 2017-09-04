/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

class CShader;

class CMesh
{
public:
	CMesh() : m_verticesVBO(0), m_uvsVBO(0) {}
	~CMesh();

	void clear();
	void render(int primitive, CShader* sh);
	void uploadToVRAM();
	void createQuad(float center_x, float center_y, float w, float h, int row, int col);
	void createGrid(int size, int scale);
	void addVertex(glm::vec3 pos, glm::vec2 uv);

	std::vector< float > m_vertices;
	std::vector< unsigned > m_verticesIndices;
	std::vector< glm::vec2 > m_uvs;
	glm::mat4 m_modelMatrix;

	unsigned int m_verticesVAO;
	unsigned int m_verticesVBO;
	unsigned int m_indicesEBO;
	unsigned int m_uvsVBO;
};
