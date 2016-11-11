#include "GameMap.h"
#include "json.h"
#include <iostream>
#include <fstream>
#include "TextureManager.h"

using json = nlohmann::json;

CGameMap::CGameMap()
{
	m_testShader = CShader::Load("data/shaders/simple.vs", "data/shaders/simple.fs");

	m_tilemap = CTextureManager::getInstance()->getTexture("data/images/tilemap.png");

	m_testShader->enable();

	if (m_tilemap != NULL)
		m_testShader->setTexture("color_texture", m_tilemap->m_texture_id);

	m_testShader->disable();
}

CGameMap::CGameMap(const std::string &mapName, int width, int height)
{
	this->m_mapName = mapName;
	this->width = width;
	this->height = height;

	m_tiles.resize(width*height);

	m_testShader = CShader::Load("data/shaders/simple.vs", "data/shaders/simple.fs");

	m_tilemap = CTextureManager::getInstance()->getTexture("data/images/tilemap.png");

	m_testShader->enable();

	if (m_tilemap != NULL)
		m_testShader->setTexture("color_texture", m_tilemap->m_texture_id);

	m_testShader->disable();
}

CGameMap::~CGameMap()
{
	for (unsigned i = 0; i < m_tiles.size(); ++i)
		delete m_tiles[i];

	m_tiles.clear();
}

bool CGameMap::readMap(const std::string &name)
{
	std::ifstream jsonMap("data/maps/" + name);

	if (!jsonMap)
		return false;

	json j(jsonMap);

	for (json::iterator it = j.begin(); it != j.end(); ++it) 
	{
		if (it.key() == "name")
		{
			json::string_t s = it.value();
			this->m_mapName = s;
		}
		if (it.key() == "mWidth")
			this->width = it.value();
		if (it.key() == "mHeight")
			this->height = it.value();

		m_tiles.resize(width*height);

		if (it.key() == "tilemaps")
		{
			if (it.value().is_null())
				continue;

			json tilemaps = it.value();

			std::string tilemapName;
			for (unsigned i = 0; i < tilemaps.size(); i++)
			{
				for (json::iterator it2 = tilemaps[i].begin(); it2 != tilemaps[i].end(); ++it2)
				{
					if (it2.key() == "tilemapPath")
					{
						json::string_t s2 = it2.value();
						tilemapName = s2;
					}
					if (it2.key() == "tiles")
					{
						json tileArray;
						tileArray = it2.value();

						for (unsigned j = 0; j < tileArray.size(); j++)
						{
							int mapX = 0;
							int mapY = 0;
							int row = 0;
							int col = 0;

							for (json::iterator it3 = tileArray[j].begin(); it3 != tileArray[j].end(); ++it3)
							{
								if (it3.key() == "mapX")
									mapX = it3.value();
								if (it3.key() == "mapY")
									mapY = it3.value();
								if (it3.key() == "tileCol")
									col = it3.value();
								if (it3.key() == "tileRow")
									row = it3.value();
							}

							CTile *tile = new CTile(mapX, mapY, row, col);
							m_tiles[width*(mapY / TILE_SIZE) + (mapX / TILE_SIZE)] = tile;
						}
					}
				}
			}
		}
	}

	std::cout << "Map " << m_mapName << " loaded" << std::endl;

	return true;
}

void CGameMap::saveMap()
{
	std::map<std::string, std::vector<CTile*>> gameMap;

	// Fill a map where the key is a tilemap name and the values all the tiles using this tilemap
	for (unsigned i = 0; i < m_tiles.size(); i++)
	{
		if (m_tiles[i] != NULL)
			gameMap[m_tiles[i]->getTilemapName()].push_back(m_tiles[i]);
	}

	json j;

	j["name"] = m_mapName;
	j["mWidth"] = width;
	j["mHeight"] = height;

	json arrayTilemaps;

	for (std::map<std::string, std::vector<CTile*>>::iterator it = gameMap.begin(); it != gameMap.end(); ++it) 
	{
		json objectTilemap;
		objectTilemap["tilemapPath"] = it->first;

		json arrayTiles;
		json objectTiles;
		for (int i = 0; i < it->second.size(); i++)
		{
			objectTiles["mapX"] = it->second[i]->getXPos();
			objectTiles["mapY"] = it->second[i]->getYPos();
			objectTiles["tileRow"] = it->second[i]->getRow();
			objectTiles["tileCol"] = it->second[i]->getCol();

			arrayTiles.push_back(objectTiles);
		}

		objectTilemap["tiles"] = arrayTiles;
		arrayTilemaps.push_back(objectTilemap);
	}

	j["tilemaps"] = arrayTilemaps;

	std::ofstream jsonMap;
	jsonMap.open("data/maps/" + m_mapName, std::ios::out | std::ios::trunc);
	jsonMap << j.dump(4);
	jsonMap.close();

	std::cout << "Map " << m_mapName << " saved" << std::endl;
}

void CGameMap::setTile(CTile *tile, int row, int col)
{
	if (row < 0 || col < 0 || row >= height || col >= width)
	{
		std::cout << "Trying to set tile outside map" << std::endl;
		return;
	}

	m_tiles[width * row + col] = tile;
}

void CGameMap::deleteTile(int row, int col)
{
	if (row < 0 || col < 0 || row >= height || col >= width)
	{
		std::cout << "Trying to delete tile outside map" << std::endl;
		return;
	}

	if (m_tiles[width * row + col] != NULL)
	{
		delete m_tiles[width * row + col];
		m_tiles[width * row + col] = NULL;
	}
}

void CGameMap::render(CCamera *camera)
{
	glm::vec3 camTraslation = glm::vec3(camera->view[3]);

	int offsetX = -camTraslation.x / TILE_SIZE;
	int offsetY = -camTraslation.y / TILE_SIZE;

	if (offsetX < 0)
		offsetX = 0;
	if (offsetY < 0)
		offsetY = 0;

	if (offsetY >= height - 20)
		offsetY = height - 20;

	if (offsetX >= width - 34)
		offsetX = width - 34;

	int maxI = (height > 20 ? 20 : height);
	int maxJ = (width > 34 ? 34 : width);
	int index = 0;

	CTile *tile;

	m_testShader->enable();

	for (unsigned i = 0; i < maxI; i++)
	{
		for (unsigned j = 0; j < maxJ; j++)
		{
			if (height <= 20 && width <= 20)
				index = width * i + j;
			else index = (width * (i + offsetY) + j) + offsetX;

			if (m_tiles[index])
			{
				tile = m_tiles[index];

				m_testShader->setMatrix44("u_mvp", camera->VP * tile->model);
				tile->quad->render(GL_TRIANGLES, m_testShader);
			}
		}
	}
}

std::string CGameMap::getName()
{
	return m_mapName;
}