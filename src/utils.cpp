#include "utils.h"

#include "includes.h"

double irand(int min, int max)
{
	return ((double)rand() / ((double)RAND_MAX + 1.0)) * (max - min) + min;
}

//this function is used to access OpenGL Extensions (special features not supported by all cards)
void* getGLProcAddress(const char* name)
{
	return SDL_GL_GetProcAddress(name);
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

Vector2 getDesktopSize(int display_index)
{
	SDL_DisplayMode current;
	// Get current display mode of all displays.
	int should_be_zero = SDL_GetCurrentDisplayMode(display_index, &current);
	return Vector2(current.w, current.h);
}

void convertCoord(int &x, int &y, int Xres, int Yres)
{
	x = (int)((x * 480.0f) / (float)Xres);
	y = (int)((y * 270.0f) / (float)Yres);
}

void getMouseRowCol(int &row, int &col, int mouseX, int mouseY)
{
	row = (int)floor(mouseY / TILE_SIZE);
	col = (int)floor(mouseX / TILE_SIZE);
}
