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
	m_camera.setOrtho(0.0f, BASE_RESOLUTION_WIDTH, BASE_RESOLUTION_HEIGHT, 0.0f, -1.0f, 1.0f, 1.0f);

	m_gridShader = CShader::Load("data/shaders/simpleColor.vs", "data/shaders/simpleColor.fs");
	m_tileSelectedShader = CShader::Load("data/shaders/simple.vs", "data/shaders/simple.fs");

	// Retreive input
	m_keystate = SDL_GetKeyboardState(NULL);

	m_tilemapSelected = CTextureManager::getInstance()->getTexture("data/tilemaps/tilemap.png");

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

	float scale = BASE_RESOLUTION_WIDTH / (float)width;

	m_window->setSize(width, height);
	glViewport(0, 0, width, height);
	m_camera.setOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f, scale);
	m_camera.aspect = width / (float)height;
}

void CEditor::update(double deltaTime)
{
	float vel = 0.1;

	if (m_keystate[SDL_SCANCODE_LSHIFT])
		vel = 0.3 * deltaTime;

	if (m_keystate[SDL_SCANCODE_W] || m_keystate[SDL_SCANCODE_UP]) m_camera.translate(0.0f, vel);
	if (m_keystate[SDL_SCANCODE_S] || m_keystate[SDL_SCANCODE_DOWN]) m_camera.translate(0.0f, -vel);
	if (m_keystate[SDL_SCANCODE_A] || m_keystate[SDL_SCANCODE_LEFT]) m_camera.translate(vel, 0.0f);
	if (m_keystate[SDL_SCANCODE_D] || m_keystate[SDL_SCANCODE_RIGHT]) m_camera.translate(-vel, 0.0f);

	m_camTraslation = m_camera.getTranslation();

	int x = (int)m_mouse_position.x;
	int y = (int)m_mouse_position.y;
	int mapRow = 0;
	int mapCol = 0;

	// Scale coordinates to current resolution
	convertCoord(x, y, m_window->mWidth, m_window->mHeight);
	// Get the row and the column of the position clicked
	getMouseRowCol(mapRow, mapCol, (int)(x - m_camTraslation.x), (int)(y - m_camTraslation.y));

	if (!m_selectedMap)
		return;

	if (m_tileSelected && !ImGui::IsMouseHoveringAnyWindow() && m_isMouseLeftPressed && (m_lastRow != mapRow || m_lastCol != mapCol))
	{
		m_tileSelected->setPos(mapCol * TILE_SIZE, mapRow * TILE_SIZE);

		CTile *newTile = new CTile();
		newTile->setValues(m_tileSelected->x, m_tileSelected->y, m_tileSelected->getRow(), m_tileSelected->getCol());
		m_selectedMap->setTile(newTile, mapRow, mapCol);
		m_lastRow = mapRow;
		m_lastCol = mapCol;
	}
	else if (m_tileSelected && ImGui::IsMouseHoveringAnyWindow() && m_isMouseLeftPressed)
	{
		delete m_tileSelected;
		m_tileSelected = NULL;
		m_tileIDSelected = -1;
	}
	else if (!ImGui::IsMouseHoveringAnyWindow() && m_isMouseRightPressed && (m_lastRow != mapRow || m_lastCol != mapCol))
	{
		m_selectedMap->deleteTile(mapRow, mapCol);
		m_lastRow = mapRow;
		m_lastCol = mapCol;
	}
}

void CEditor::render()
{
	glClearColor(0.92f, 0.90f, 0.94f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_selectedMap)
		m_selectedMap->render(&m_camera);

	if (m_tileSelected && !ImGui::IsMouseHoveringAnyWindow())
	{
		m_tileSelectedShader->enable();
		m_tileSelectedShader->setMatrix44("u_mvp", m_camera.VP * m_tileSelected->m_modelMatrix);
		m_tileSelected->m_quad->render(GL_TRIANGLES, m_tileSelectedShader);
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

	if (ImGui::Button("New Map"))
		ImGui::OpenPopup("New Map");
	if (ImGui::BeginPopupModal("New Map", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
	{
		static char mName[60] = "";
		static int width = 0;
		static int height = 0;

		ImGui::InputText("Map Name", mName, 60);
		ImGui::InputInt("Map Width", &width);
		ImGui::InputInt("Map Height", &height);

		ImGui::Separator();

		if (ImGui::Button("Create"))
		{
			std::string mapName(mName);
			mapName += ".json";

			if (mapName == "" || width <= 1 || height <= 1)
			{
				std::cout << "The map name is empty or the size is incorrect" << std::endl;
			}
			else
			{
				// Create the map
				addMap(mapName, width, height);

				// Reset ImGui variables
				mName[0] = '\0';
				width = 0;
				height = 0;
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();

			// Reset ImGui variables
			mName[0] = '\0';
			width = 0;
			height = 0;
		}

		ImGui::EndPopup();
	}

	ImGui::Separator();

	ImGui::Combo("Select Map", &m_currentMapIndex,
		[](void* vec, int idx, const char** out_text) {
		std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= vector->size()) return false;
		*out_text = vector->at(idx).c_str();
		return true;
	}, reinterpret_cast<void*>(&m_createdMaps), m_createdMaps.size());

	if (!m_selectedMap && m_currentMapIndex != -1)
	{
		loadMap();
		setGrid();
	}
	else if (m_selectedMap && m_selectedMap->getName() != m_createdMaps[m_currentMapIndex])
	{
		if (m_gameMaps.count(m_createdMaps[m_currentMapIndex]))
		{
			m_selectedMap = m_gameMaps[m_createdMaps[m_currentMapIndex]];
			setCameraCenter(m_selectedMap->width, m_selectedMap->height);
		}
		else
		{
			loadMap();
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

	ImGui::SameLine();

	if (ImGui::Button("Delete") && m_selectedMap)
		ImGui::OpenPopup("Delete?");
	if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
	{
		ImGui::Text("Are you sure you want to delete the selected map?\n\n");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			deleteMap();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
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
	for (int i = 0; i < TILEMAP_SIZE; i++)
	{
		for (int j = 0; j < TILEMAP_SIZE; j++)
		{
			ImGui::PushID(ID);
			int frame_padding = 2;

			// Change color if tile has been clicked
			if (m_tileIDSelected == ID) style.Colors[ImGuiCol_Button] = ImColor(240, 5, 5, 255);
			else style.Colors[ImGuiCol_Button] = ImColor(240, 240, 240, 255);

			if (ImGui::ImageButton((void*)m_tilemapSelected->m_texture_id, ImVec2(35.0f, 35.0f), ImVec2((1.0f / TILEMAP_SIZE) * j, (1.0f / TILEMAP_SIZE) * i),
				ImVec2((1.0f / TILEMAP_SIZE) * j + 1.0f / TILEMAP_SIZE, (1.0f / TILEMAP_SIZE) * i + 1.0f / TILEMAP_SIZE), frame_padding, ImColor(0, 0, 0, 255)))
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

	m_gridMesh.uploadToVRAM();
}

void CEditor::loadMap()
{
	CGameMap *newMap;
	newMap = new CGameMap();
	newMap->readMap(m_createdMaps[m_currentMapIndex]);
	m_gameMaps[newMap->getName()] = newMap;

	m_selectedMap = newMap;
	setCameraCenter(m_selectedMap->width, m_selectedMap->height);
}

void CEditor::addMap(const std::string &mapName, int width, int height)
{
	CGameMap *newMap;
	newMap = new CGameMap(mapName, width, height);
	newMap->saveMap();

	m_gameMaps[mapName] = newMap;
	m_currentMapIndex = m_createdMaps.size();
	m_selectedMap = newMap;

	setGrid();
	m_showGrid = true;

	// Center the camera to the center of the map
	setCameraCenter(m_selectedMap->width, m_selectedMap->height);

	// Add to map names list
	m_createdMaps.push_back(newMap->getName());

	// Add map to file with map names
	std::ofstream jsonMap;
	jsonMap.open("data/maps/mapList.txt", std::ios::out | std::ios::app);
	jsonMap << newMap->getName() << "\n";
	jsonMap.close();
}

void CEditor::deleteMap()
{
	// Delete from name list
	std::vector<std::string>::iterator it2;
	it2 = std::find(m_createdMaps.begin(), m_createdMaps.end(), m_selectedMap->getName());
	m_createdMaps.erase(it2);

	// Delete from maps container
	std::map<std::string, CGameMap *>::iterator it;
	it = m_gameMaps.find(m_selectedMap->getName());
	m_gameMaps.erase(it);

	std::cout << "Map " << m_selectedMap->getName() << " deleted" << std::endl;;

	delete m_selectedMap;
	m_selectedMap = NULL;
	m_currentMapIndex = -1;

	// Rewrite the file with map names
	std::ofstream jsonMap;
	jsonMap.open("data/maps/mapList.txt", std::ios::out | std::ios::trunc);
	for (unsigned i = 0; i < m_createdMaps.size(); ++i)
	{
		jsonMap << m_createdMaps[i] << "\n";
	}
	jsonMap.close();
}

void CEditor::setCameraPos(float x, float y)
{
	m_camera.setPosition(glm::vec3(x, y, 0.0f));
}

void CEditor::setCameraCenter(int width, int height)
{
	setCameraPos(m_camera.right * 0.5f * m_camera.scale - width * 0.5f * TILE_SIZE, m_camera.bottom * 0.5f * m_camera.scale - height * 0.5f * TILE_SIZE);
}