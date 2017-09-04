#pragma once

#include "includes.h"
#include <glm/glm.hpp>

class CInputHandler
{
public:
	static CInputHandler& getInstance()
	{
		static CInputHandler singleton;
		return singleton;
	}

	void handleInput();
	bool isPressed(int key);

	bool m_isMouseRightPressed;
	bool m_isMouseLeftPressed;
	bool m_running;
	bool m_resizeWindow;

	const Uint8* m_keystate;

	int lastWidth;
	int lastHeight;
	glm::vec2 m_mouse_position;
	int m_mouse_state;

private:
	CInputHandler() : m_isMouseLeftPressed(false), m_isMouseRightPressed(false), m_running(true), 
		lastWidth(BASE_RESOLUTION_WIDTH), lastHeight(BASE_RESOLUTION_HEIGHT)
	{
		m_keystate = SDL_GetKeyboardState(NULL); 
	
	}
	SDL_Event sdlEvent;
};