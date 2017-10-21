/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include <iostream>
#include <chrono>
#include <memory>
#include <windows.h>

#include <src/graphics/window.h>
#include <src/graphics/sprite.h>
#include <src/graphics/shader.h>
#include <src/graphics/renderer.h>
#include <src/graphics/texturearray.h>
#include <src/utils/debugutils.h>

#include <src/graphics/text.h>

extern "C" 
{
	// NVIDIA Optimus: Default dGPU instead of iGPU (Driver: 302+)
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	// AMD: Request dGPU High Performance (Driver: 13.35+)
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main(int argc, char* argv[])
{
	// Everything contained here is for testing porpuses
	std::shared_ptr<gfx::Window> window = std::make_shared<gfx::Window>();

	window->init("Game Newell", 1280, 720);
	window->setClearColor(225, 208, 130);

	double deltaTime = 0;

	input::Keyboard &input = input::Keyboard::getInstance();

	gfx::Renderer renderer;

	gfx::Text text("Hello World", math::vec2(500, 500), math::vec2(200, 100));

	gfx::Sprite sprite(math::vec2(100, 100), math::vec2(128, 128), renderer.getTexture("data/sprites/face.png"));
	gfx::Sprite sprite2(math::vec2(100, 400), math::vec2(128, 128), renderer.getTexture("data/sprites/faceSad.png"));
	gfx::Sprite sprite3(math::vec2(500, 100), math::vec2(256, 256), renderer.getTexture("data/sprites/bigFace.png"));

	gfx::Shader shaderTest("src/graphics/shaders/instancedQuad.vs", "src/graphics/shaders/instancedQuad.fs");

	while (!window->isClosed() && !input.isPressed(GLFW_KEY_ESCAPE))
	{
		renderer.addRenderable(sprite);
		renderer.addRenderable(sprite2);
		renderer.addRenderable(sprite3);

		window->clear();

		shaderTest.enable();

		shaderTest.setMatrix4("uProjection", math::mat4::ortho(0.0f, (float)window->getWidth(), (float)window->getHeight(), 0.0f, -1.0f, 1.0f));

		renderer.render();

		window->update();

		utils::printGlErrors();
	}

	return 0;
}
