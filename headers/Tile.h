/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "texture.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"
#include <glm\glm.hpp>

class CTile
{
public:
	CTile() : x(0), y(0), row(0), col(0), m_quad(nullptr) {}
	CTile(float x, float y, int row, int col);
	~CTile();

	void update(double deltaTime);
	void setValues(float x, float y, int row, int col);
	void setPos(float x, float y);
	int getRow();
	int getCol();
	std::string getTilemapName();

	CMesh* m_quad;
	glm::mat4 m_modelMatrix;
	float x;
	float y;

private:
	CTexture *m_tilemap;

	/**
	* The row and column to start reading UVs from the tilemap
	*/
	int row;
	int col;
};
