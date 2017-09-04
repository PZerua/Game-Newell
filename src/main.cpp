/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "Window.h"
#include "includes.h"
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include <chrono>
#include <windows.h>
#include "Game.h"
#include "InputHandler.h"

extern "C" 
{
	// NVIDIA Optimus: Default dGPU instead of iGPU (Driver: 302+)
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	// AMD: Request dGPU High Performance (Driver: 13.35+)
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


using namespace std::chrono;

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

	// Turn nanoseconds into milliseconds
	delta /= 1000000.0;

	timePrev = timeCurrent;

	return delta;
}

void mainLoop(CGame *game)
{
	CInputHandler &input = CInputHandler::getInstance();
	int x, y;
	SDL_GetMouseState(&x, &y);

	input.m_mouse_position = glm::vec2(x, y);
	double deltaTime = 0;

	while (input.m_running)
	{
		input.handleInput();
		//get mouse position and delta (do after pump events)
		input.m_mouse_state = SDL_GetMouseState(&x, &y);
		//editor->mouse_delta.set(game->mouse_position.x - x, game->mouse_position.y - y);
		input.m_mouse_position = glm::vec2(x, y);

		//update logic
		deltaTime = GetDelta();
		game->update(deltaTime);
		game->render();
	}
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	std::shared_ptr<CWindow> window = std::make_shared<CWindow>();

	bool fullscreen = false;
	glm::vec2 size(800, 720); // 5 times 160x144

	if (fullscreen)
		size = getDesktopSize(0);
	window->init("Game Newell", size.x, size.y, fullscreen);

	std::clog << glGetString(GL_VERSION) << std::endl;

	ImGui_ImplSdlGL3_Init(window->mWindow);

	std::unique_ptr<CGame> game = std::make_unique<CGame>(window);

	mainLoop(game.get());

	ImGui_ImplSdlGL3_Shutdown();
	SDL_Quit();

	return 0;
}