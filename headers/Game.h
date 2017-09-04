/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "includes.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Tile.h"
#include "TextureManager.h"
#include <fstream>
#include "Sprite.h"
#include "Editor.h"
#include "InputHandler.h"

class CGame
{
public:
	CGame(std::shared_ptr<CWindow> window);
	~CGame() {}

	void update(double deltaTime);
	void render();
	void setWindowSize(int width, int height);

private:
	std::unique_ptr<CEditor> m_editor;
	std::shared_ptr<CCamera> m_camera;
	std::shared_ptr<CWindow> m_window;

};