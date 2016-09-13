#pragma once

#include "camera.h"
#include "Window.h"
#include "includes.h"
#include "mesh.h"
#include "GameMap.h"
#include "utils.h"
#include "imgui.h"

class Editor
{
public:
	Editor(Window *window);
	~Editor();
	void init();
	void render();
	void renderImGui();
	void update(double deltaTime);
	Vector2 mouse_position;
	const Uint8* keystate;
	void onKeyPressed(SDL_KeyboardEvent event);
	void onMouseButtonDown(SDL_MouseButtonEvent event);
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void setWindowSize(int width, int height);
	void drawGrid();
	int mouse_state;
	void setGrid();
	Window *window;

private:
	GameMap *selectedMap;
	std::vector<GameMap *> gameMaps;
	Camera *camera;
	Mesh *gridMesh;
	Shader *gridShader;
	Shader *tileSelectedShader;
	Tile *tileSelected;
	ImVec2 editorSize;
	Vector3 camTraslation;

	bool showGrid;
	bool isMouseRightPressed;
	bool isMouseLeftPressed;
	int tileIDSelected;
	int lastRow;
	int lastCol;
};
