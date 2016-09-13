#pragma once

#include <vector>
#include "Framework.h"

class Shader;

class Mesh
{
public:
	std::vector< Vector3 > vertices;
	std::vector< Vector2 > uvs;

	unsigned int vertices_vbo_id;
	unsigned int uvs_vbo_id;

	Mesh();
	Mesh(const Mesh& m);
	~Mesh();

	void clear();
	void render(int primitive, Shader* sh);

	void uploadToVRAM();
	
	void createQuadUVs(float center_x, float center_y, float w, float h, int row, int col);
	void createQuad(float center_x, float center_y, float w, float h);
	void setUVs(int row, int col);
};
