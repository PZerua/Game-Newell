#include "Tile.h"
#include "TextureManager.h"

Tile::Tile()
{
	this->x = 0;
	this->y = 0;
	this->row = 0;
	this->col = 0;

	quad = NULL;
}

Tile::Tile(float x, float y, int row, int col)
{
	this->x = x;
	this->y = y;
	this->row = row;
	this->col = col;

	model.setTranslation(x, y, 0);

	quad = new Mesh();
	quad->createQuadUVs(TILE_SIZE * 0.5, TILE_SIZE * 0.5, TILE_SIZE, TILE_SIZE, row, col);

	TextureManager *manager;
	tilemap = manager->getInstance()->getTexture("data/images/tilemap.png");

	quad->uploadToVRAM();
}

Tile::~Tile()
{
	delete quad;
}

void Tile::setValues(float x, float y, int row, int col)
{
	this->x = x;
	this->y = y;
	this->row = row;
	this->col = col;

	model.setTranslation(x, y, 0);

	quad = new Mesh();
	quad->createQuadUVs(TILE_SIZE * 0.5, TILE_SIZE * 0.5, TILE_SIZE, TILE_SIZE, row, col);

	quad->uploadToVRAM();

	TextureManager *manager;
	tilemap = manager->getInstance()->getTexture("data/images/tilemap.png");

}


void Tile::update(double deltaTime)
{

}

std::string Tile::getTilemapName()
{
	return tilemap->filename;
}

float Tile::getXPos()
{
	return x;
}

float Tile::getYPos()
{
	return y;
}

int Tile::getRow()
{
	return row;
}

int Tile::getCol()
{
	return col;
}

void Tile::setPos(float x, float y)
{
	this->x = x;
	this->y = y;
	model.setTranslation(x, y, 0);
}