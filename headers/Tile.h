#pragma once

#include "texture.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"

class Tile
{
public:
	Tile();
	Tile(float x, float y, int row, int col);
	~Tile();
	void update(double deltaTime);
	void setValues(float x, float y, int row, int col);
	std::string getTilemapName();
	float getXPos();
	float getYPos();
	int getRow();
	int getCol();
	void setPos(float x, float y);
	Mesh* quad;
	Matrix44 model;

private:
	Texture *tilemap;
	int row;
	int col;
	float x;
	float y;
};
