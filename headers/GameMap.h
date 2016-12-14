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

	/**
	* Read map from Json file using map name
	*/
	bool readMap(const std::string &name);

	/**
	* Save Json map
	*/
	void saveMap();

	void init();
	void setTile(CTile *tile, int row, int col);
	std::string getName();
	void deleteTile(int row, int col);
	void render(CCamera *camara);
	int width;
	int height;

private:
	std::string m_mapName;
	std::vector<CTile *> m_tiles;
	CShader *m_testShader;
	CTexture *m_tilemap;
	CMesh* m_quad;
};
