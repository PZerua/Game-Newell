/**
* (c) 2016 Pablo Luis Garc�a. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "Window.h"
#include "Editor.h"
#include "imgui_impl_sdl_gl3.h"
#include <chrono>

using namespace std::chrono;

CEditor *editor;

auto timePrev = high_resolution_clock::now();

// Returns time since last time this function was called in seconds with nanosecond precision
double GetDelta()
{
	// Gett current time as a std::chrono::time_point
	// which basically contains info about the current point in time
	auto timeCurrent = high_resolution_clock::now();

	// Compare the two to create time_point containing delta time in nanosecnds
	auto timeDiff = duration_cast<nanoseconds>(timeCurrent - timePrev);

	// Get the tics as a variable
	double delta = (double)timeDiff.count();

	// Turn nanoseconds into seconds
	delta /= 1000000.0;

	timePrev = timeCurrent;

	return delta;
}

void mainLoop()
{
	SDL_Event sdlEvent;
	int x, y;

	SDL_GetMouseState(&x, &y);
	editor->m_mouse_position = glm::vec2(x, y);
	double deltaTime = 0;

	while (editor->m_running)
	{
		//update events
		while (SDL_PollEvent(&sdlEvent))
		{
			ImGui_ImplSdlGL3_ProcessEvent(&sdlEvent);

			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				return;
			case SDL_MOUSEBUTTONDOWN:
				editor->onMouseButtonDown(sdlEvent.button);
				break;
			case SDL_MOUSEBUTTONUP:
				editor->onMouseButtonUp(sdlEvent.button);
				break;
			case SDL_KEYDOWN:
				editor->onKeyPressed(sdlEvent.key);
				break;
			case SDL_WINDOWEVENT:
				switch (sdlEvent.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					editor->setWindowSize(sdlEvent.window.data1, sdlEvent.window.data2);
					break;
				}
			}
		}

		//get mouse position and delta (do after pump events)
		editor->m_mouse_state = SDL_GetMouseState(&x, &y);
		//editor->mouse_delta.set(game->mouse_position.x - x, game->mouse_position.y - y);
		editor->m_mouse_position = glm::vec2(x, y);

		//update logic
		deltaTime = GetDelta();
		editor->update(deltaTime);
		editor->render();
	}
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	std::shared_ptr<CWindow> window = std::make_shared<CWindow>();

	bool fullscreen = false;
	glm::vec2 size(1280, 720);

	if (fullscreen)
		size = getDesktopSize(0);
	window->init("Game Newell", size.x, size.y, fullscreen);

	std::clog << glGetString(GL_VERSION) << std::endl;

	ImGui_ImplSdlGL3_Init(window->mWindow);

	editor = new CEditor(window);
	editor->init();

	mainLoop();

	ImGui_ImplSdlGL3_Shutdown();
	SDL_Quit();

	return 0;
}