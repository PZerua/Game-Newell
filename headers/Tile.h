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
	CTile() : x(0), y(0), row(0), col(0), quad(NULL) {}
	CTile(float x, float y, int row, int col);
	~CTile();
	void update(double deltaTime);
	void setValues(float x, float y, int row, int col);
	std::string getTilemapName();
	float getXPos();
	float getYPos();
	int getRow();
	int getCol();
	void setPos(float x, float y);
	CMesh* quad;
	glm::mat4 model;

private:
	CTexture *m_tilemap;
	int row;
	int col;
	float x;
	float y;
};
