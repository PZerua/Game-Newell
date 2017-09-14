/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include <iostream>
#include <chrono>
#include <memory>
#include <windows.h>

#include <src/graphics/window.h>
#include <src/graphics/renderable2d.h>
#include <src/graphics/shader.h>
#include <src/math/mat4.h>
#include <src/utils/debugutils.h>

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
	glViewport(0, 0, 960, 864);
	glDisable(GL_CULL_FACE);

	double deltaTime = 0;

	input::Keyboard &input = input::Keyboard::getInstance();

	gfx::Shader shaderTest("src/graphics/shaders/simple.vs", "src/graphics/shaders/simple.fs");

	gfx::Renderable2D test(math::vec2(480, 432), math::vec2(200, 200), math::vec4(1, 0, 0, 1));
	
	while (!window->isClosed() && !input.isPressed(GLFW_KEY_ESCAPE))
	{		
		window->clear();

		shaderTest.enable();

		shaderTest.setMatrix4("u_mvp", math::mat4::ortho(0.0f, 960.0f, 864.0f, 0.0f, -3000.0f, 3000.0f) * test.m_modelMatrix);
		//shaderTest.setMatrix4("u_mvp", math::mat4::perspective(80, 960.0f / 864.0f, 0.0f, 10000.0f) * test.m_modelMatrix);

		test.m_vao->bind();

		glDrawElements(GL_TRIANGLES, QUAD_INDICES_SIZE, GL_UNSIGNED_SHORT, 0);

		test.m_vao->unbind();

		shaderTest.disable();

		window->update();

		deltaTime = GetDelta();

		utils::printGlErrors();
	}

	return 0;
}