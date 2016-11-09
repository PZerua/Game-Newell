#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

class CShader;

class CMesh
{
public:
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;

	unsigned int vertices_vbo_id;
	unsigned int uvs_vbo_id;

	CMesh() : vertices_vbo_id(0), uvs_vbo_id(0) {}
	~CMesh();

	void clear();
	void render(int primitive, CShader* sh);

	void uploadToVRAM();
	
	void createQuad(float center_x, float center_y, float w, float h, int row, int col);
	void setUVs(int row, int col);
};
