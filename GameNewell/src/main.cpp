/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
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
#include <src/utils/timer.h>

#include <src/editor/gui/imgui.h>
#include <src/editor/gui/imgui_impl_glfw_gl3.h>
#include <src/math/math.h>
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

    ImGui_ImplGlfwGL3_Init(window->getWindow(), false);

    double deltaTime = 0;

    input::Keyboard &input = input::Keyboard::getInstance();

    gfx::Renderer renderer;

    gfx::Text text("Hello World", math::vec2(500, 500), math::vec2(200, 100));

    gfx::Sprite sprite(math::vec2(100, 100), math::vec2(8, 8), renderer.getTexture("data/sprites/smallFace.png"));
    gfx::Sprite sprite2(math::vec2(100, 400), math::vec2(16, 16), renderer.getTexture("data/sprites/faceSad.png"));
    gfx::Sprite sprite3(math::vec2(500, 100), math::vec2(16, 16), renderer.getTexture("data/sprites/bigFace.png"));

    gfx::Shader shaderTest("src/graphics/shaders/instancedQuad.vs", "src/graphics/shaders/instancedQuad.fs");

    math::mat4 ortho = math::mat4::ortho(0.0f, (float)window->getWidth(), (float)window->getHeight(), 0.0f, -1.0f, 1.0f);

    while (!window->isClosed() && !input.isPressed(GLFW_KEY_ESCAPE))
    {
        window->pollEvents();

        window->clear();

        ImGui_ImplGlfwGL3_NewFrame();

        ImGui::Begin("Debug", (bool *)true);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        // Add 14400 sprites
        for (int i = 0; i < 160; i++)
        {
            for (int j = 0; j < 90; j++)
            {
                sprite.setTranslation(math::vec2(8 * i, 8 * j));
                renderer.addRenderable(sprite);
            }
        }

        // Add ~1800 sprites
        for (int i = 0; i < 80; i++)
        {
            for (int j = 0; j < 22; j++)
            {
                sprite2.setTranslation(math::vec2(16 * i, 16 * j));
                renderer.addRenderable(sprite2);
            }
        }

        // Add ~1800 sprites
        for (int i = 0; i < 80; i++)
        {
            for (int j = 22; j < 45; j++)
            {
                sprite3.setTranslation(math::vec2(16 * i, 16 * j));
                renderer.addRenderable(sprite3);
            }
        }

        shaderTest.enable();

        shaderTest.setMatrix4("uProjection", ortho);

        renderer.render();

        ImGui::Render();

        window->swap();

        utils::printGlErrors();
    }

    return 0;
}
