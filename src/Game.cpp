/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "Game.h"

CGame::CGame(std::shared_ptr<CWindow> window)
{
	m_window = window;

	m_camera = std::make_shared<CCamera>();
	m_camera->setViewport(0, 0, window->mWidth, window->mHeight);
	m_camera->setOrtho(0.0f, BASE_RESOLUTION_WIDTH, BASE_RESOLUTION_HEIGHT, 0.0f, -1.0f, 1.0f, 1.0f);

	m_editor = std::make_unique<CEditor>(window, m_camera);
	m_editor->init();
}

void CGame::setWindowSize(int width, int height)
{
	std::clog << "window resized: " << width << "," << height << std::endl;

	float scale = BASE_RESOLUTION_WIDTH / (float)width;

	m_window->setSize(width, height);
	glViewport(0, 0, width, height);
	m_camera->setOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f, scale);
	m_camera->aspect = width / (float)height;
}

void CGame::update(double deltaTime)
{
	m_editor->update(deltaTime);

	CInputHandler input = CInputHandler::getInstance();

	if (input.m_resizeWindow)
	{
		setWindowSize(input.lastWidth, input.lastHeight);
	}
}

void CGame::render()
{
	m_editor->render();
}
