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
	CGameMap();
	CGameMap(const std::string &mapName, int width, int height);
	~CGameMap();
	bool readMap(const std::string &name);
	void saveMap();
	void setTile(CTile *tile, int row, int col);
	void deleteTile(int row, int col);
	void render(CCamera *camara);
	int width;
	int height;
	std::string getName();

private:
	std::string m_mapName;
	std::vector<CTile *> m_tiles;
	CShader *m_testShader;
	CTexture *m_tilemap;
	CMesh* m_quad;
};
