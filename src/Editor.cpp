#include "Editor.h"
#include "imgui_impl_sdl_gl3.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "Tile.h"
#include "TextureManager.h"
#include <iostream>
#include <fstream>

CEditor::~CEditor()
{
	delete m_window;
	delete gridShader;
	delete tileSelected;
	delete tilemapSelected;
	delete[] keystate;
}

void CEditor::init()
{
	camera.setViewport(0, 0, m_window->mWidth, m_window->mHeight);
	camera.setOrtho(0.0f, 480.0f, 270.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	gridShader = CShader::Load("data/shaders/simpleColor.vs", "data/shaders/simpleColor.fs");
	tileSelectedShader = CShader::Load("data/shaders/simple.vs", "data/shaders/simple.fs");

	// Retreive input
	keystate = SDL_GetKeyboardState(NULL);

	tilemapSelected = CTextureManager::getInstance()->getTexture("data/images/tilemap.png");

	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	/*if (window->mWidth < current.w && window->mHeight < current.h)
		setWindowSize(1280, 720);*/

	std::ifstream mapList("data/maps/mapList.txt");

	std::string line;

	while (mapList >> line)
	{
		maps.push_back(line);
	}

	if (maps.size() != 0)
	{
		CGameMap *newMap;
		newMap = new CGameMap();
		newMap->readMap(maps[0]);
		gameMaps[maps[0]] = newMap;
		selectedMap = newMap;
		setGrid();
		setCameraPos(camera.right * 0.5 * camera.scale - selectedMap->width * 0.5 * TILE_SIZE, camera.bottom * 0.5 * camera.scale - selectedMap->height * 0.5 * TILE_SIZE);
	}

	currentMapID = 0;
}

void CEditor::onKeyPressed(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE: exit(0);
	}
}

void CEditor::onMouseButtonDown(SDL_MouseButtonEvent event)
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

void CEditor::onMouseButtonUp(SDL_MouseButtonEvent event)
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

void CEditor::setWindowSize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;

	float scale = 480.0f / (double)width;

	m_window->setSize(width, height);
	glViewport(0, 0, width, height);
	camera.setOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f, scale);
	camera.aspect = width / (float)height;
}

void CEditor::update(double deltaTime)
{
	float vel = 0.1f * deltaTime;

	if (keystate[SDL_SCANCODE_LSHIFT])
		vel = 0.3f * deltaTime;

	if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) camera.translate(0.0f, vel);
	if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) camera.translate(0.0f, -vel);
	if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) camera.translate(vel, 0.0f);
	if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) camera.translate(-vel, 0.0f);

	camTraslation = glm::vec3(camera.view[3]);

	int x = (int)mouse_position.x;
	int y = (int)mouse_position.y;
	int row = 0;
	int col = 0;
	convertCoord(x, y, m_window->mWidth, m_window->mHeight);
	getMouseRowCol(row, col, (int)(x - camTraslation.x), (int)(y - camTraslation.y));

	if (!selectedMap)
		return;

	if (tileSelected && !ImGui::IsMouseHoveringAnyWindow() && isMouseLeftPressed && (lastRow != row || lastCol != col))
	{
		tileSelected->setPos(col * TILE_SIZE, row * TILE_SIZE);

		CTile *newTile = new CTile();
		newTile->setValues(tileSelected->getXPos(), tileSelected->getYPos(), tileSelected->getRow(), tileSelected->getCol());
		selectedMap->setTile(newTile, row, col);
		lastRow = row;
		lastCol = col;
	}
	else if (tileSelected && ImGui::IsMouseHoveringAnyWindow() && isMouseLeftPressed)
	{
		delete tileSelected;
		tileSelected = NULL;
		tileIDSelected = -1;
	}
	else if (!ImGui::IsMouseHoveringAnyWindow() && isMouseRightPressed && (lastRow != row || lastCol != col))
	{
		selectedMap->deleteTile(row, col);
		lastRow = row;
		lastCol = col;
	}
}

void CEditor::render()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (selectedMap)
		selectedMap->render(&camera);

	if (tileSelected && !ImGui::IsMouseHoveringAnyWindow())
	{
		tileSelectedShader->enable();
		tileSelectedShader->setMatrix44("u_mvp", camera.VP * tileSelected->model);
		tileSelected->quad->render(GL_TRIANGLES, tileSelectedShader);
	}

	if (showGrid)
		drawGrid();

	renderImGui();

	SDL_GL_SwapWindow(m_window->mWindow);
}

void CEditor::renderImGui()
{
	ImGui_ImplSdlGL3_NewFrame(m_window->mWindow);

	ImGui::SetNextWindowPos(ImVec2(m_window->mWidth - editorSize.x, m_window->mHeight * 0.5f - editorSize.y * 0.5f));
	ImGui::Begin("Editor", (bool *)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::CollapsingHeader("New Map"))
	{
		static char mName[60] = "";
		static int width = 0;
		static int height = 0;

		ImGui::InputText("Map Name", mName, 60);
		ImGui::InputInt("Map Width", &width);
		ImGui::InputInt("Map Height", &height);

		if (ImGui::Button("Create Map"))
		{
			std::string mapName(mName);
			mapName += ".json";

			if (mapName == "" || width <= 1 || height <= 1)
			{
				std::cout << "The map name is empty or the size is incorrect" << std::endl;
			}
			else
			{
				CGameMap *newMap;
				newMap = new CGameMap(mapName, width, height);
				gameMaps[mapName] = newMap;
				currentMapID = maps.size();

				selectedMap = newMap;
				newMap->saveMap();
				setCameraPos(camera.right * 0.5 - selectedMap->width * 0.5 * TILE_SIZE, camera.bottom * 0.5 - selectedMap->height * 0.5 * TILE_SIZE);
				setGrid();
				showGrid = true;
				mName[0] = '\0';
				width = 0;
				height = 0;
				ImGui::SetNextTreeNodeOpen(false);

				maps.push_back(newMap->getName());

				std::ofstream jsonMap;
				jsonMap.open("data/maps/mapList.txt", std::ios::out | std::ios::app);
				jsonMap << newMap->getName() << "\n";
				jsonMap.close();
			}
		}
	}

	ImGui::Separator();

	ImGui::Combo("Select Map", &currentMapID,
		[](void* vec, int idx, const char** out_text) {
		std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= vector->size()) return false;
		*out_text = vector->at(idx).c_str();
		return true;
	}, reinterpret_cast<void*>(&maps), maps.size());

	if (selectedMap && maps.size() != 0 && selectedMap->getName() != maps[currentMapID])
	{
		if (gameMaps.count(maps[currentMapID]))
		{
			selectedMap = gameMaps[maps[currentMapID]];
			setCameraPos(camera.right * 0.5 * camera.scale - selectedMap->width * 0.5 * TILE_SIZE, camera.bottom * 0.5 * camera.scale - selectedMap->height * 0.5 * TILE_SIZE);
		}
		else
		{
			CGameMap *newMap;
			newMap = new CGameMap();
			newMap->readMap(maps[currentMapID]);
			gameMaps[newMap->getName()] = newMap;

			selectedMap = newMap;
			setCameraPos(camera.right * 0.5 * camera.scale - selectedMap->width * 0.5 * TILE_SIZE, camera.bottom * 0.5 * camera.scale - selectedMap->height * 0.5 * TILE_SIZE);
		}
		setGrid();
	}

	ImGui::Separator();

	ImGui::Text("Map Options:");

	ImGui::Checkbox("Show Grid", &showGrid);

	ImGui::SameLine();

	if (ImGui::Button("Save Map"))
	{
		if (selectedMap)
			selectedMap->saveMap();
	}

	ImGui::Separator();

	ImGui::Spacing();

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

			if (ImGui::ImageButton((void*)tilemapSelected->texture_id, ImVec2(35.0f, 35.0f), ImVec2((1.0f / 8.0f) * j, (1.0f / 8.0f) * i),
				ImVec2((1.0f / 8.0f) * j + 1.0f / 8.0f, (1.0f / 8.0f) * i + 1.0f / 8.0f), frame_padding, ImColor(0, 0, 0, 255)))
			{
				tileIDSelected = ID;
				int x = (int)mouse_position.x;
				int y = (int)mouse_position.y;
				convertCoord(x, y, m_window->mWidth, m_window->mHeight);
				tileSelected = new CTile(x + TILE_SIZE * 0.5f, y + TILE_SIZE * 0.5f, i, j);
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

void CEditor::drawGrid()
{
	if (!selectedMap)
		return;

	gridShader->enable();
	gridShader->setVector3("color", glm::vec3(0.25f, 0.35f, 0.45f));
	gridShader->setMatrix44("u_mvp", camera.VP);
	gridMesh.render(GL_LINES, gridShader);
}

void CEditor::setGrid()
{
	glm::vec3 tmp;
	gridMesh.clear();
	// Create grid
	for (int i = 0; i < selectedMap->height + 1; i++)
	{
		tmp = glm::vec3(0, i * TILE_SIZE, 0);
		gridMesh.vertices.push_back(tmp);
		tmp = glm::vec3(selectedMap->width * TILE_SIZE, i * TILE_SIZE, 0);
		gridMesh.vertices.push_back(tmp);
	}

	for (int i = 0; i < selectedMap->width + 1; i++)
	{
		tmp = glm::vec3(i * TILE_SIZE, 0, 0);
		gridMesh.vertices.push_back(tmp);
		tmp = glm::vec3(i * TILE_SIZE, selectedMap->height * TILE_SIZE, 0);
		gridMesh.vertices.push_back(tmp);
	}
}

void CEditor::setCameraPos(float x, float y)
{
	camera.setPosition(glm::vec3(x, y, 0.0f));
}