#pragma once

#include <glm\glm.hpp>
#include <vector>

glm::vec2 getDesktopSize(int display_index = 0);

double irand(int min, int max);
void convertCoord(int &x, int &y, int Xres, int Yres);
void getMouseRowCol(int &row, int &col, int mouseX, int mouseY);
std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);