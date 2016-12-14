/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "camera.h"
#include "Window.h"
#include "includes.h"
#include "mesh.h"
#include "GameMap.h"
#include "utils.h"
#include "imgui.h"

class CEditor
{
public:
	CEditor(CWindow *window) : m_window(window), m_lastRow(-1), m_lastCol(-1), m_showGrid(false), m_selectedMap(NULL),
		m_isMouseLeftPressed(false), m_isMouseRightPressed(false), m_tileSelected(NULL), m_tileIDSelected(-1), m_currentMapIndex(-1) {}
	~CEditor();
	void init();
	void render();
	void renderImGui();
	void update(double deltaTime);
	void onKeyPressed(SDL_KeyboardEvent event);
	void onMouseButtonDown(SDL_MouseButtonEvent event);
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void setWindowSize(int width, int height);
	void drawGrid();
	void setGrid();
	void setCameraPos(float x, float y);
	void setCameraCenter(int width, int height);
	void loadMap();
	void addMap(const std::string &mapName, int width, int height);
	void deleteMap();
	CWindow *m_window;
	glm::vec2 m_mouse_position;
	const Uint8* m_keystate;
	int m_mouse_state;

private:
	CTexture *m_tilemapSelected;
	CGameMap *m_selectedMap;
	std::map<std::string, CGameMap *> m_gameMaps;
	CCamera m_camera;
	CMesh m_gridMesh;
	CShader *m_gridShader;
	CShader *m_tileSelectedShader;
	CTile *m_tileSelected;
	ImVec2 m_editorSize;
	glm::vec3 m_camTraslation;
	std::vector<std::string> m_createdMaps;

	bool m_showGrid;
	bool m_isMouseRightPressed;
	bool m_isMouseLeftPressed;
	int m_tileIDSelected;
	int m_lastRow;
	int m_lastCol;
	int m_currentMapIndex;
};
