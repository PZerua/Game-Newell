/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "includes.h"
#include "GameMap.h"
#include "Sprite.h"
#include <fstream>

class CWorld
{
public:
	static CWorld& getInstance()
	{
		static CWorld singleton;
		return singleton;
	}

	std::shared_ptr<CGameMap> loadMap();
	std::shared_ptr<CGameMap> addMap(const std::string &mapName, int width, int height);
	void deleteMap();

	std::unique_ptr<CSprite> m_mainCharacter;
	std::vector<std::string> m_createdMaps;
	std::map<std::string, std::shared_ptr<CGameMap>> m_gameMaps;

	int m_currentMapIndex;

private:
	CWorld();
};