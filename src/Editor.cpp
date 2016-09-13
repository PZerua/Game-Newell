#include "Editor.h"
#include "imgui_impl_sdl_gl3.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "Tile.h"
#include "TextureManager.h"

Texture *text;

Editor::Editor(Window *window)
{
	this->window = window;
}

Editor::~Editor()
{
	delete window;
	delete camera;
	delete gridMesh;
	delete gridShader;
	delete tileSelected;
	delete[] keystate;

	delete text;
}

void Editor::init()
{
	camera = new Camera();
	camera->setOrthographic(0.0f, 480.0f, 270.0f, 0.0f, -1.0f, 1.0f);

	gridShader = Shader::Load("data/shaders/simpleColor.vs", "data/shaders/simpleColor.fs");
	tileSelectedShader = Shader::Load("data/shaders/simple.vs", "data/shaders/simple.fs");
	tileIDSelected = -1;
	tileSelected = NULL;
	isMouseLeftPressed = false;
	isMouseRightPressed = false;
	lastRow = -1;
	lastCol = -1;
	selectedMap = NULL;

	// Retreive input
	keystate = SDL_GetKeyboardState(NULL);

	// Grid mesh
	gridMesh = new Mesh();
	showGrid = true;
	Vector3 vert;
	
	TextureManager *manager;

	text = manager->getInstance()->getTexture("data/images/tilemap.png");
	
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	if (window->mWidth < current.w && window->mHeight < current.h)
		setWindowSize(1280, 720);
}

void Editor::onKeyPressed(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE: exit(0);
	}
}

void Editor::onMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_MIDDLE)
	{
	}
	if (event.button == SDL_BUTTON_RIGHT)
	{
		isMouseRightPressed = true;
	}
	if (event.button == SDL_BUTTON_LEFT)
	{
		isMouseLeftPressed = true;
	}
}

void Editor::onMouseButtonUp(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_MIDDLE)
	{
	}
	if (event.button == SDL_BUTTON_RIGHT)
	{
		isMouseRightPressed = false;
	}
	if (event.button == SDL_BUTTON_LEFT)
	{
		isMouseLeftPressed = false;
	}
}

void Editor::setWindowSize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;

	window->setSize(width, height);
	glViewport(0, 0, width, height);
	camera->aspect = width / (float)height;
}

void Editor::update(double deltaTime)
{
	
	float vel = 0.1f * deltaTime;

	if (keystate[SDL_SCANCODE_LSHIFT])
		vel = 0.3f * deltaTime;

	if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) camera->view_matrix.traslate(0.0f, vel, 0.0f);
	if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) camera->view_matrix.traslate(0.0f, -vel, 0.0f);
	if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) camera->view_matrix.traslate(vel, 0.0f, 0.0f);
	if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) camera->view_matrix.traslate(-vel, 0.0f, 0.0f);

	camTraslation = camera->view_matrix.getTraslation();

	int x = (int)mouse_position.x;
	int y = (int)mouse_position.y;
	int row = 0;
	int col = 0;
	convertCoord(x, y, window->mWidth, window->mHeight);
	getMouseRowCol(row, col, (int)(x - camTraslation.x), (int)(y - camTraslation.y));

	if (selectedMap == NULL)
		return;

	if (tileSelected != NULL && !ImGui::IsMouseHoveringAnyWindow())
	{
		tileSelected->setPos(col * TILE_SIZE, row * TILE_SIZE);

		if (isMouseLeftPressed && (lastRow != row || lastCol != col))
		{
			Tile *newTile = new Tile();
			newTile->setValues(tileSelected->getXPos(), tileSelected->getYPos(), tileSelected->getRow(), tileSelected->getCol());
			selectedMap->setTile(newTile, row, col);
			lastRow = row;
			lastCol = col;
		}
	}
	else if (tileSelected != NULL && ImGui::IsMouseHoveringAnyWindow() && isMouseLeftPressed)
	{
		delete tileSelected;
		tileSelected = NULL;
		tileIDSelected = -1;
	}
	else if (!ImGui::IsMouseHoveringAnyWindow() && isMouseRightPressed && (lastRow != row || lastCol != col))
	{
		lastRow = row;
		lastCol = col;

		selectedMap->deleteTile(row, col);
	}
}

void Editor::render()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->set();

	if (selectedMap != NULL)
		selectedMap->render(camera);

	if (tileSelected != NULL && !ImGui::IsMouseHoveringAnyWindow())
	{
		tileSelectedShader->enable();
		tileSelectedShader->setMatrix44("u_mvp", tileSelected->model * camera->viewprojection_matrix);
		tileSelected->quad->render(GL_TRIANGLES, tileSelectedShader);
	}

	if (showGrid) drawGrid();

	renderImGui();

	SDL_GL_SwapWindow(window->mWindow);
}

void Editor::renderImGui()
{
	ImGui_ImplSdlGL3_NewFrame(window->mWindow);

	ImGui::SetNextWindowPos(ImVec2(window->mWidth - editorSize.x, window->mHeight * 0.5f - editorSize.y * 0.5f));
	ImGui::Begin("Editor", (bool *)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::Checkbox("Show Grid", &showGrid);

	if (ImGui::CollapsingHeader("Create Map"))
	{
		GameMap *newMap;

		static char mName[60] = "";
		static int width = 0;
		static int height = 0;
		
		ImGui::InputText("Map Name: ", mName, 60);
		ImGui::InputInt("Map Width: ", &width);
		ImGui::InputInt("Map Height: ", &height);

		if (ImGui::Button("Submit")) 
		{
			newMap = new GameMap(mName, width, height);
			gameMaps.push_back(newMap);

			selectedMap = newMap;
			setGrid();
			showGrid = true;
		}
	}

	if(ImGui::Button("Save Map"))
	{
		selectedMap->saveMap();
	}

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 itemspacing = style.ItemSpacing;
	ImVec4 color = style.Colors[ImGuiCol_Button];
	ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
	ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];

	// Change frame colors and the space between buttons
	style.ItemSpacing.x = 0;
	style.ItemSpacing.y = 0;
	style.Colors[ImGuiCol_Button] = ImColor(240, 240, 240, 255);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(240, 5, 5, 255);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(240, 5, 5, 255);

	int ID = 0;
	// Tile selector
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			ImGui::PushID(ID);
			int frame_padding = 2;

			// Change color if tile has been clicked
			if (tileIDSelected == ID) style.Colors[ImGuiCol_Button] = ImColor(240, 5, 5, 255);
			else style.Colors[ImGuiCol_Button] = ImColor(240, 240, 240, 255);

			if (ImGui::ImageButton((void*)text->texture_id, ImVec2(35, 35), ImVec2((1.0f / 8.0f) * j, (1.0f / 8.0f) * i), 
				ImVec2((1.0f / 8.0f) * j + 1.0f / 8.0f, (1.0f / 8.0f) * i + 1.0f / 8.0f), frame_padding, ImColor(0, 0, 0, 255)))
			{
				tileIDSelected = ID;
				int x = (int)mouse_position.x;
				int y = (int)mouse_position.y;
				convertCoord(x, y, window->mWidth, window->mHeight);
				tileSelected = new Tile(x + TILE_SIZE * 0.5f, y + TILE_SIZE * 0.5f, i, j);
			}

			ImGui::PopID();
			ImGui::SameLine();
			ID++;
		}
		ImGui::Spacing();
	}

	// Restore default values
	style.ItemSpacing = itemspacing;
	style.Colors[ImGuiCol_Button] = color;
	style.Colors[ImGuiCol_ButtonActive] = colorActive;
	style.Colors[ImGuiCol_ButtonHovered] = colorHover;

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	editorSize = ImGui::GetWindowSize();

	ImGui::End();
	ImGui::Render();
}

void Editor::drawGrid()
{
	if (selectedMap == NULL)
		return;

	gridShader->enable();

	gridShader->setVector3("color", Vector3(0.25f, 0.35f, 0.45f));
	gridShader->setMatrix44("u_mvp", camera->viewprojection_matrix);
	gridMesh->render(GL_LINES, gridShader);

	gridShader->disable();
}

void Editor::setGrid()
{
	Vector3 vert;
	// Create grid
	for (int i = 0; i < selectedMap->height + 1; i++)
	{
		vert = Vector3(0, i * TILE_SIZE, 0);
		gridMesh->vertices.push_back(vert);
		vert = Vector3(selectedMap->width * TILE_SIZE, i * TILE_SIZE, 0);
		gridMesh->vertices.push_back(vert);
	}

	for (int i = 0; i < selectedMap->width + 1; i++)
	{
		vert = Vector3(i * TILE_SIZE, 0, 0);
		gridMesh->vertices.push_back(vert);
		vert = Vector3(i * TILE_SIZE, selectedMap->width * TILE_SIZE, 0);
		gridMesh->vertices.push_back(vert);
	}
}