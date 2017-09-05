#include "InputHandler.h"
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

void CInputHandler::handleInput()
{
	//update events
	while (SDL_PollEvent(&sdlEvent))
	{
		ImGui_ImplSdlGL3_ProcessEvent(&sdlEvent);

		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (sdlEvent.button.button == SDL_BUTTON_RIGHT) m_isMouseRightPressed = true;
			if (sdlEvent.button.button == SDL_BUTTON_LEFT) m_isMouseLeftPressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (sdlEvent.button.button == SDL_BUTTON_RIGHT) m_isMouseRightPressed = false;
			if (sdlEvent.button.button == SDL_BUTTON_LEFT) m_isMouseLeftPressed = false;
			break;
		case SDL_KEYDOWN:
			switch (sdlEvent.key.keysym.sym)
			{
			case SDLK_ESCAPE: m_running = false;
			}
			break;
		case SDL_WINDOWEVENT:
			switch (sdlEvent.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				m_resizeWindow = true;
				lastWidth = sdlEvent.window.data1;
				lastHeight = sdlEvent.window.data2;
				break;
			}
		}
	}
}

bool CInputHandler::isPressed(int key)
{
	return m_keystate[key];
}