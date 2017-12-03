/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#include <iostream>
#include <chrono>
#include <memory>
#include <windows.h>

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
#include <src/graphics/shader.h>


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
    input::Keyboard &input = input::Keyboard::getInstance();

    gfx::Renderer renderer;

    ImGui_ImplGlfwGL3_Init(renderer.getWindow()->getGlfwWindow(), false);

    gfx::Text text("Hello World", math::vec2(500, 500), math::vec2(200, 100));

    gfx::Renderable2D sprite(math::vec2(100, 100), math::vec2(8, 8), renderer.getTexture("smallFace"), renderer.getShader("defaultInstanced2D"));
    gfx::Renderable2D sprite2(math::vec2(100, 400), math::vec2(16, 16), renderer.getTexture("faceSad"), renderer.getShader("defaultInstanced2D"));
    gfx::Renderable2D sprite3(math::vec2(500, 100), math::vec2(16, 16), renderer.getTexture("bigFace"), renderer.getShader("defaultInstanced2D"));

    while (!renderer.getWindow()->isClosed() && !input.isPressed(GLFW_KEY_ESCAPE))
    {
        renderer.getWindow()->pollEvents();
        renderer.getWindow()->clear();

        ImGui_ImplGlfwGL3_NewFrame();

        ImGui::Begin("Debug", (bool *)true);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        // Add 14400 sprites
        for (int i = 0; i < 160; i++)
        {
            for (int j = 0; j < 90; j++)
            {
                sprite.setTranslation(math::vec2(8.0f * i, 8.0f * j));
                renderer.addRenderable(sprite);
            }
        }

        // Add ~1800 sprites
        for (int i = 0; i < 80; i++)
        {
            for (int j = 0; j < 22; j++)
            {
                sprite2.setTranslation(math::vec2(16.0f * i, 16.0f * j));
                renderer.addRenderable(sprite2);
            }
        }

        // Add ~1800 sprites
        for (int i = 0; i < 80; i++)
        {
            for (int j = 22; j < 45; j++)
            {
                sprite3.setTranslation(math::vec2(16.0f * i, 16.0f * j));
                renderer.addRenderable(sprite3);
            }
        }

        renderer.render();

        ImGui::Render();

        renderer.getWindow()->swap();

        utils::printGlErrors();
    }

    return 0;
}
