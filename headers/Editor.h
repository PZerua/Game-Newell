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
	CEditor(CWindow *window) : m_window(window), lastRow(-1), lastCol(-1), showGrid(true), selectedMap(NULL), 
		isMouseLeftPressed(false), isMouseRightPressed(false), tileSelected(NULL), tileIDSelected(-1) {}
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
	CWindow *m_window;
	glm::vec2 mouse_position;
	const Uint8* keystate;
	int mouse_state;

private:
	CTexture *tilemapSelected;
	CGameMap *selectedMap;
	std::map<std::string, CGameMap *> gameMaps;
	CCamera camera;
	CMesh gridMesh;
	CShader *gridShader;
	CShader *tileSelectedShader;
	CTile *tileSelected;
	ImVec2 editorSize;
	glm::vec3 camTraslation;
	std::vector<std::string> maps;

	bool showGrid;
	bool isMouseRightPressed;
	bool isMouseLeftPressed;
	int tileIDSelected;
	int lastRow;
	int lastCol;
	int currentMapID;
};
