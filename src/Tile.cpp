/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "Tile.h"
#include "TextureManager.h"

CTile::CTile(float x, float y, int row, int col) : x(x), y(y), row(row), col(col)
{
	model = glm::translate(model, glm::vec3(x, y, 0));

	quad = new CMesh();
	quad->createQuad(TILE_SIZE * 0.5, TILE_SIZE * 0.5, TILE_SIZE, TILE_SIZE, row, col);

	CTextureManager *manager;
	m_tilemap = manager->getInstance()->getTexture("data/images/tilemap.png");

	quad->uploadToVRAM();
}

CTile::~CTile()
{
	delete quad;
}

void CTile::setValues(float x, float y, int row, int col)
{
	this->x = x;
	this->y = y;
	this->row = row;
	this->col = col;

	model = glm::translate(model, glm::vec3(x, y, 0));

	if (quad)
		delete quad;

	quad = new CMesh();
	quad->createQuad(TILE_SIZE * 0.5, TILE_SIZE * 0.5, TILE_SIZE, TILE_SIZE, row, col);

	quad->uploadToVRAM();

	m_tilemap = CTextureManager::getInstance()->getTexture("data/images/tilemap.png");
}

void CTile::update(double deltaTime)
{
}

std::string CTile::getTilemapName()
{
	return m_tilemap->m_filename;
}

float CTile::getXPos()
{
	return x;
}

float CTile::getYPos()
{
	return y;
}

int CTile::getRow()
{
	return row;
}

int CTile::getCol()
{
	return col;
}

void CTile::setPos(float x, float y)
{
	this->x = x;
	this->y = y;
	model = glm::translate(model, glm::vec3(x, y, 0));
}