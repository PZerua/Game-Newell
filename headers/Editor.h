/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "includes.h"
#include "Camera.h"
#include "Window.h"
#include "Mesh.h"
#include "GameMap.h"
#include "utils.h"
#include "imgui.h"
#include "InputHandler.h"
#include "Shader.h"
#include "Texture.h"
#include "World.h"
#include "Tile.h"

class CEditor
{
public:
	CEditor(std::shared_ptr<CWindow> window, std::shared_ptr<CCamera> camera, std::shared_ptr<CGameMap> currentMap) : m_window(window), 
		m_camera(camera), m_lastRow(-1), m_lastCol(-1), m_showGrid(false), m_tileIDSelected(-1), m_canMove(true), m_selectedMap(currentMap) {}
	~CEditor() {}
	void init();
	void render();
	void renderImGui();
	void update(double deltaTime);
	void drawGrid();
	void setGrid();
	void setCameraPos(float x, float y);
	void setCameraCenter(int width, int height);
	void loadMap();
	void addMap(const std::string &mapName, int width, int height);
	void deleteMap();

private:
	CInputHandler *input;
	CWorld *m_world;
	
	ImVec2 m_editorSize;
	CMesh m_gridMesh;
	// Temporal addresses to current selected objects
	std::weak_ptr<CTexture> m_tilemapSelected;
	std::unique_ptr<CTile> m_tileSelected;
	std::shared_ptr<CGameMap> m_selectedMap;
	std::shared_ptr<CCamera> m_camera;
	std::shared_ptr<CShader> m_gridShader;
	std::shared_ptr<CShader> m_tileSelectedShader;
	std::shared_ptr<CWindow> m_window;
	glm::vec3 m_camTraslation;

	bool m_showGrid;
	bool m_canMove;
	int m_tileIDSelected;
	int m_lastRow;
	int m_lastCol;
};
