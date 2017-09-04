/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "GameMap.h"
#include "json.h"
#include <iostream>
#include <fstream>
#include "TextureManager.h"
#include "utils.h"

using json = nlohmann::json;

CGameMap::CGameMap(const std::string &mapName, int width, int height)
{
	this->m_mapName = mapName;
	this->width = width;
	this->height = height;

	m_tiles.resize(width*height);

	init();
}

void CGameMap::init()
{
	m_mapShader = std::make_shared<CShader>("data/shaders/simple.vs", "data/shaders/simple.fs");

	m_tilemap = CTextureManager::getInstance().getTexture("data/tilemaps/tilemap.png");
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

		if (width && height)
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

							// Save tile in vector
							m_tiles[width*(mapY / TILE_SIZE) + (mapX / TILE_SIZE)] = std::make_unique<CTile>(mapX, mapY, row, col);
						}
					}
				}
			}
		}
	}

	std::clog << "Map " << m_mapName << " loaded" << std::endl;

	return true;
}

void CGameMap::saveMap()
{
	std::map<std::string, std::vector<CTile*>> gameMap;

	// Fill a map where the key is a tilemap name and the values all the tiles using this tilemap
	for (unsigned i = 0; i < m_tiles.size(); i++)
	{
		if (m_tiles[i] != NULL)
			gameMap[m_tiles[i]->getTilemapName()].push_back(m_tiles[i].get());
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
			objectTiles["mapX"] = it->second[i]->x;
			objectTiles["mapY"] = it->second[i]->y;
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

void CGameMap::setTile(int x, int y, int tilemapRow, int tilemapCol, int mapRow, int mapCol)
{
	if (mapRow < 0 || mapCol < 0 || mapRow >= height || mapCol >= width)
	{
		std::cerr << "Trying to set tile outside map" << std::endl;
		return;
	}

	m_tiles[width * mapRow + mapCol] = std::make_unique<CTile>(x, y, tilemapRow, tilemapCol);
}

std::string CGameMap::getName()
{
	return m_mapName;
}

void CGameMap::deleteTile(int row, int col)
{
	if (row < 0 || col < 0 || row >= height || col >= width)
	{
		std::cerr << "Trying to delete tile outside map" << std::endl;
		return;
	}

	if (m_tiles[width * row + col] != NULL)
	{
		m_tiles[width * row + col].release();
	}
}

void CGameMap::render(CCamera *camera)
{
	glm::vec3 camTranslation = camera->getTranslation();

	int startX = (int)(-camTranslation.x / TILE_SIZE);
	int startY = (int)(-camTranslation.y / TILE_SIZE);
	int endX = (int)((-camTranslation.x + camera->right) / TILE_SIZE + 1);
	int endY = (int)((-camTranslation.y + camera->bottom) / TILE_SIZE + 1);

	startX = clip(startX, 0, width);
	startY = clip(startY, 0, height);
	endX = clip(endX, 0, width);
	endY = clip(endY, 0, height);

	CTile *tile;

	m_mapShader->enable();

	if (m_tilemap != NULL)
		glBindTexture(GL_TEXTURE_2D, m_tilemap->m_texture_id);

	for (int i = startY; i < endY; i++)
	{
		for (int j = startX; j < endX; j++)
		{
			if (m_tiles[width * i + j])
			{
				tile = m_tiles[width * i + j].get();

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				m_mapShader->setMatrix4("u_mvp", camera->VP * tile->m_modelMatrix);
				tile->m_quad->render(GL_TRIANGLES, m_mapShader.get());

				glDisable(GL_BLEND);
			}
		}
	}
}