#include "World.h"

CWorld::CWorld() : m_currentMapIndex(-1)
{
	m_mainCharacter = std::make_unique<CSprite>("data/spritesheets/spriteSheet.png");

	std::ifstream mapList("data/maps/mapList.txt");

	std::string line;

	while (mapList >> line)
	{
		m_createdMaps.push_back(line);
	}
}

std::shared_ptr<CGameMap> CWorld::loadMap()
{
	std::shared_ptr<CGameMap> newMap = std::make_shared<CGameMap>();
	newMap->readMap(m_createdMaps[m_currentMapIndex]);
	m_gameMaps[newMap->getName()] = newMap;

	return newMap;
}

std::shared_ptr<CGameMap> CWorld::addMap(const std::string &mapName, int width, int height)
{
	std::shared_ptr<CGameMap> newMap = std::make_shared<CGameMap>(mapName, width, height);
	newMap->saveMap();

	m_gameMaps[mapName] = newMap;
	m_currentMapIndex = m_createdMaps.size();

	// Add to map names list
	m_createdMaps.push_back(newMap->getName());

	// Add map to file with map names
	std::ofstream jsonMap;
	jsonMap.open("data/maps/mapList.txt", std::ios::out | std::ios::app);
	jsonMap << newMap->getName() << "\n";
	jsonMap.close();

	return newMap;
}

void CWorld::deleteMap()
{
	if (m_currentMapIndex == -1)
		return;

	// Delete from name list
	std::vector<std::string>::iterator it2;
	it2 = std::find(m_createdMaps.begin(), m_createdMaps.end(), m_createdMaps[m_currentMapIndex]);
	m_createdMaps.erase(it2);

	// Delete from maps container
	std::map<std::string, std::shared_ptr<CGameMap>>::iterator it;
	it = m_gameMaps.find(m_createdMaps[m_currentMapIndex]);
	std::clog << "Map " << m_createdMaps[m_currentMapIndex] << " deleted" << std::endl;;
	m_gameMaps.erase(it);

	m_currentMapIndex = -1;

	// Rewrite the file with map names
	std::ofstream jsonMap;
	jsonMap.open("data/maps/mapList.txt", std::ios::out | std::ios::trunc);
	for (unsigned i = 0; i < m_createdMaps.size(); ++i)
	{
		jsonMap << m_createdMaps[i] << "\n";
	}
	jsonMap.close();
}