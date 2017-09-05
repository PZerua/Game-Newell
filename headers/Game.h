/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "includes.h"
#include "Sprite.h"
#include "Editor.h"
#include "InputHandler.h"
#include "World.h"

class CGame
{
public:
	CGame(std::shared_ptr<CWindow> window);
	~CGame() {}

	void update(double deltaTime);
	void render();
	void setWindowSize(int width, int height);

private:
	CWorld *m_world;
	std::unique_ptr<CEditor> m_editor;
	std::shared_ptr<CCamera> m_camera;
	std::shared_ptr<CWindow> m_window;
	std::shared_ptr<CGameMap> m_currentMap;

};