#include "Window.h"
#include "Editor.h"
#include "imgui_impl_sdl_gl3.h"

float P2M = 60.0;
float M2P = 1.0 / P2M;
//b2World *world;
Editor *editor;

void mainLoop()
{
	SDL_Event sdlEvent;
	int x, y;

	SDL_GetMouseState(&x, &y);
	editor->mouse_position.set(x, y);
	double deltaTime = 0;

	while (1)
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
				switch (sdlEvent.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					editor->setWindowSize(sdlEvent.window.data1, sdlEvent.window.data2);
					break;
				}
			}
		}

		//get mouse position and delta (do after pump events)
		editor->mouse_state = SDL_GetMouseState(&x, &y);
		//editor->mouse_delta.set(game->mouse_position.x - x, game->mouse_position.y - y);
		editor->mouse_position.set(x, y);


		//update logic
		deltaTime = 1000.0f / ImGui::GetIO().Framerate;
		editor->update(deltaTime);
		editor->render();
	}
}

/*
void addRect(int x, int y, int w, int h, bool dyn = true)
{
	b2BodyDef bodydef;
	bodydef.position.Set(x*P2M, y*P2M);
	if (dyn)
		bodydef.type = b2_dynamicBody;
	b2Body* body = world->CreateBody(&bodydef);

	b2PolygonShape shape;
	shape.SetAsBox(P2M*w / 2, P2M*h / 2);

	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = 1.0;
	body->CreateFixture(&fixturedef);
}
*/

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	Window *window;
	window = new Window();

	float fullscreen = false;
	Vector2 size(480, 270);

	if (fullscreen)
		size = getDesktopSize(0);

	window->init("MAP EDITOR", size.x, size.y, fullscreen);

	ImGui_ImplSdlGL3_Init(window->mWindow);

	editor = new Editor(window);
	editor->init();

	std::cout << glGetString(GL_VERSION) << std::endl;

	mainLoop();

	SDL_Quit();

	return 0;
}
