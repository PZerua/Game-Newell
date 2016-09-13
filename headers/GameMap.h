#pragma once

#include "includes.h"
#include "Tile.h"
#include "camera.h"

class GameMap
{
public:
	GameMap(const std::string &mapName, int width, int height);
	~GameMap();
	bool readMap();
	void saveMap();
	void setTile(Tile *tile, int row, int col);
	void deleteTile(int row, int col);
	void render(Camera *camara);
	int width;
	int height;

private:
	std::string mapName;
	std::vector<Tile *> tiles;
	Shader *testShader;
	Texture *tilemap;
	Mesh* quad;

};
