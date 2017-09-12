/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include <iostream>
#include <chrono>
#include <memory>
#include <windows.h>

#include <src/graphics/window.h>

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

int main(int argc, char* argv[])
{
	std::shared_ptr<gfx::Window> window = std::make_shared<gfx::Window>();

	window->init("Game Newell", 960, 864);
	glClearColor(0.15f, 0.20f, 0.26f, 1.0f);

	double deltaTime = 0;

	input::Keyboard &input = input::Keyboard::getInstance();

	while (!window->isClosed() && !input.isPressed(GLFW_KEY_ESCAPE))
	{		
		window->clear();
		window->update();

		deltaTime = GetDelta();
	}

	return 0;
}