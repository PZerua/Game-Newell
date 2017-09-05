/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "includes.h"
#include "Tile.h"
#include "camera.h"

class CGameMap
{
public:
	CGameMap() : width(0), height(0) { init(); }
	CGameMap(const std::string &mapName, int width, int height);
	~CGameMap() {};

	/**
	* Read map from Json file using map name
	*/
	bool readMap(const std::string &name);

	/**
	* Save Json map
	*/
	void saveMap();

	void init();
	void setTile(int x, int y, int tilemapX, int tilemapY, int mapRow, int mapCol);
	std::string getName();
	void deleteTile(int row, int col);
	void render(CCamera *camara, int row = -1, int col = -1);
	int width;
	int height;

private:
	std::string m_mapName;
	std::vector<std::unique_ptr<CTile>> m_tiles;
	std::shared_ptr<CShader> m_mapShader;
	std::shared_ptr<CTexture> m_tilemap;
	std::unique_ptr<CMesh> m_quad;
};
