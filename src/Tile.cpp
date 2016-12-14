/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "Tile.h"
#include "TextureManager.h"

CTile::CTile(float x, float y, int row, int col) : x(x), y(y), row(row), col(col)
{
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(x, y, 0));

	m_quad = new CMesh();
	m_quad->createQuad(TILE_SIZE * 0.5, TILE_SIZE * 0.5, TILE_SIZE, TILE_SIZE, row, col);

	CTextureManager *manager;
	m_tilemap = manager->getInstance()->getTexture("data/tilemaps/tilemap.png");
}

CTile::~CTile()
{
	delete m_quad;
}

void CTile::setValues(float x, float y, int row, int col)
{
	this->x = x;
	this->y = y;
	this->row = row;
	this->col = col;

	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(x, y, 0));

	if (m_quad)
		delete m_quad;

	m_quad = new CMesh();
	m_quad->createQuad(TILE_SIZE * 0.5, TILE_SIZE * 0.5, TILE_SIZE, TILE_SIZE, row, col);

	m_tilemap = CTextureManager::getInstance()->getTexture("data/tilemaps/tilemap.png");
}

void CTile::update(double deltaTime)
{
}

std::string CTile::getTilemapName()
{
	return m_tilemap->m_filename;
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
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(x, y, 0));
}