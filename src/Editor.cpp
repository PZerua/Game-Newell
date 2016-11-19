/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

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
	delete m_gridShader;
	delete m_tileSelected;
	delete m_tilemapSelected;
	delete m_tileSelectedShader;
	delete[] m_keystate;
}

void CEditor::init()
{
	m_camera.setViewport(0, 0, m_window->mWidth, m_window->mHeight);
	m_camera.setOrtho(0.0f, 480.0f, 270.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	m_gridShader = CShader::Load("data/shaders/simpleColor.vs", "data/shaders/simpleColor.fs");
	m_tileSelectedShader = CShader::Load("data/shaders/simple.vs", "data/shaders/simple.fs");

	// Retreive input
	m_keystate = SDL_GetKeyboardState(NULL);

	m_tilemapSelected = CTextureManager::getInstance()->getTexture("data/images/tilemap.png");

	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	/*if (window->mWidth < current.w && window->mHeight < current.h)
		setWindowSize(1280, 720);*/

	std::ifstream mapList("data/maps/mapList.txt");

	std::string line;

	while (mapList >> line)
	{
		m_createdMaps.push_back(line);
	}
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
		m_isMouseRightPressed = true;
	}
	if (event.button == SDL_BUTTON_LEFT)
	{
		m_isMouseLeftPressed = true;
	}
}

void CEditor::onMouseButtonUp(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_MIDDLE)
	{
	}
	if (event.button == SDL_BUTTON_RIGHT)
	{
		m_isMouseRightPressed = false;
	}
	if (event.button == SDL_BUTTON_LEFT)
	{
		m_isMouseLeftPressed = false;
	}
}

void CEditor::setWindowSize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;

	float scale = 480.0f / (float)width;

	m_window->setSize(width, height);
	glViewport(0, 0, width, height);
	m_camera.setOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f, scale);
	m_camera.aspect = width / (float)height;
}

void CEditor::update(double deltaTime)
{
	float vel = 0.1 * deltaTime;

	if (m_keystate[SDL_SCANCODE_LSHIFT])
		vel = 0.3 * deltaTime;

	if (m_keystate[SDL_SCANCODE_W] || m_keystate[SDL_SCANCODE_UP]) m_camera.translate(0.0f, vel);
	if (m_keystate[SDL_SCANCODE_S] || m_keystate[SDL_SCANCODE_DOWN]) m_camera.translate(0.0f, -vel);
	if (m_keystate[SDL_SCANCODE_A] || m_keystate[SDL_SCANCODE_LEFT]) m_camera.translate(vel, 0.0f);
	if (m_keystate[SDL_SCANCODE_D] || m_keystate[SDL_SCANCODE_RIGHT]) m_camera.translate(-vel, 0.0f);

	m_camTraslation = glm::vec3(m_camera.view[3]);

	int x = (int)m_mouse_position.x;
	int y = (int)m_mouse_position.y;
	int row = 0;
	int col = 0;
	convertCoord(x, y, m_window->mWidth, m_window->mHeight);
	getMouseRowCol(row, col, (int)(x - m_camTraslation.x), (int)(y - m_camTraslation.y));

	if (!m_selectedMap)
		return;

	if (m_tileSelected && !ImGui::IsMouseHoveringAnyWindow() && m_isMouseLeftPressed && (m_lastRow != row || m_lastCol != col))
	{
		m_tileSelected->setPos(col * TILE_SIZE, row * TILE_SIZE);

		CTile *newTile = new CTile();
		newTile->setValues(m_tileSelected->getXPos(), m_tileSelected->getYPos(), m_tileSelected->getRow(), m_tileSelected->getCol());
		m_selectedMap->setTile(newTile, row, col);
		m_lastRow = row;
		m_lastCol = col;
	}
	else if (m_tileSelected && ImGui::IsMouseHoveringAnyWindow() && m_isMouseLeftPressed)
	{
		delete m_tileSelected;
		m_tileSelected = NULL;
		m_tileIDSelected = -1;
	}
	else if (!ImGui::IsMouseHoveringAnyWindow() && m_isMouseRightPressed && (m_lastRow != row || m_lastCol != col))
	{
		m_selectedMap->deleteTile(row, col);
		m_lastRow = row;
		m_lastCol = col;
	}
}

void CEditor::render()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_selectedMap)
		m_selectedMap->render(&m_camera);

	if (m_tileSelected && !ImGui::IsMouseHoveringAnyWindow())
	{
		m_tileSelectedShader->enable();
		m_tileSelectedShader->setMatrix44("u_mvp", m_camera.VP * m_tileSelected->model);
		m_tileSelected->quad->render(GL_TRIANGLES, m_tileSelectedShader);
	}

	if (m_showGrid)
		drawGrid();

	renderImGui();

	SDL_GL_SwapWindow(m_window->mWindow);
}

void CEditor::renderImGui()
{
	ImGui_ImplSdlGL3_NewFrame(m_window->mWindow);

	ImGui::SetNextWindowPos(ImVec2(m_window->mWidth - m_editorSize.x, m_window->mHeight * 0.5f - m_editorSize.y * 0.5f));
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
				m_gameMaps[mapName] = newMap;
				m_currentMapID = m_createdMaps.size();
				m_selectedMap = newMap;
				newMap->saveMap();
				setGrid();
				m_showGrid = true;

				// Center the camera to the center of the map
				setCameraCenter(m_selectedMap->width, m_selectedMap->height);

				// Reset ImGui variables
				mName[0] = '\0';
				width = 0;
				height = 0;
				ImGui::SetNextTreeNodeOpen(false);

				m_createdMaps.push_back(newMap->getName());

				std::ofstream jsonMap;
				jsonMap.open("data/maps/mapList.txt", std::ios::out | std::ios::app);
				jsonMap << newMap->getName() << "\n";
				jsonMap.close();
			}
		}
	}

	ImGui::Separator();

	ImGui::Combo("Select Map", &m_currentMapID,
		[](void* vec, int idx, const char** out_text) {
		std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= vector->size()) return false;
		*out_text = vector->at(idx).c_str();
		return true;
	}, reinterpret_cast<void*>(&m_createdMaps), m_createdMaps.size());

	if (!m_selectedMap && m_currentMapID != -1)
	{
		addMap();
		setGrid();
	}
	else if (m_selectedMap && m_selectedMap->getName() != m_createdMaps[m_currentMapID])
	{
		if (m_gameMaps.count(m_createdMaps[m_currentMapID]))
		{
			m_selectedMap = m_gameMaps[m_createdMaps[m_currentMapID]];
			setCameraCenter(m_selectedMap->width, m_selectedMap->height);
		}
		else
		{
			addMap();
		}
		setGrid();
	}

	ImGui::Separator();

	ImGui::Text("Map Options:");

	ImGui::Checkbox("Show Grid", &m_showGrid);

	ImGui::SameLine();

	if (ImGui::Button("Save Map"))
	{
		if (m_selectedMap)
			m_selectedMap->saveMap();
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
			if (m_tileIDSelected == ID) style.Colors[ImGuiCol_Button] = ImColor(240, 5, 5, 255);
			else style.Colors[ImGuiCol_Button] = ImColor(240, 240, 240, 255);

			if (ImGui::ImageButton((void*)m_tilemapSelected->m_texture_id, ImVec2(35.0f, 35.0f), ImVec2((1.0f / 8.0f) * j, (1.0f / 8.0f) * i),
				ImVec2((1.0f / 8.0f) * j + 1.0f / 8.0f, (1.0f / 8.0f) * i + 1.0f / 8.0f), frame_padding, ImColor(0, 0, 0, 255)))
			{
				m_tileIDSelected = ID;
				int x = (int)m_mouse_position.x;
				int y = (int)m_mouse_position.y;
				convertCoord(x, y, m_window->mWidth, m_window->mHeight);
				m_tileSelected = new CTile(x + TILE_SIZE * 0.5f, y + TILE_SIZE * 0.5f, i, j);
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

	m_editorSize = ImGui::GetWindowSize();

	ImGui::End();
	ImGui::Render();
}

void CEditor::drawGrid()
{
	if (!m_selectedMap)
		return;

	m_gridShader->enable();
	m_gridShader->setVector3("color", glm::vec3(0.25f, 0.35f, 0.45f));
	m_gridShader->setMatrix44("u_mvp", m_camera.VP);
	m_gridMesh.render(GL_LINES, m_gridShader);
}

void CEditor::setGrid()
{
	glm::vec3 tmp;
	m_gridMesh.clear();
	// Create grid
	for (int i = 0; i < m_selectedMap->height + 1; i++)
	{
		tmp = glm::vec3(0, i * TILE_SIZE, 0);
		m_gridMesh.m_vertices.push_back(tmp);
		tmp = glm::vec3(m_selectedMap->width * TILE_SIZE, i * TILE_SIZE, 0);
		m_gridMesh.m_vertices.push_back(tmp);
	}

	for (int i = 0; i < m_selectedMap->width + 1; i++)
	{
		tmp = glm::vec3(i * TILE_SIZE, 0, 0);
		m_gridMesh.m_vertices.push_back(tmp);
		tmp = glm::vec3(i * TILE_SIZE, m_selectedMap->height * TILE_SIZE, 0);
		m_gridMesh.m_vertices.push_back(tmp);
	}
}

void CEditor::addMap()
{
	CGameMap *newMap;
	newMap = new CGameMap();
	newMap->readMap(m_createdMaps[m_currentMapID]);
	m_gameMaps[newMap->getName()] = newMap;

	m_selectedMap = newMap;
	setCameraCenter(m_selectedMap->width, m_selectedMap->height);
}

void CEditor::setCameraPos(float x, float y)
{
	m_camera.setPosition(glm::vec3(x, y, 0.0f));
}

void CEditor::setCameraCenter(int width, int height)
{
	setCameraPos(m_camera.right * 0.5f * m_camera.scale - width * 0.5f * TILE_SIZE, m_camera.bottom * 0.5f * m_camera.scale - height * 0.5f * TILE_SIZE);
}