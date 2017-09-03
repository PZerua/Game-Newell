/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <vector>
#include "glm/glm.hpp"

glm::vec2 getDesktopSize(int display_index = 0);

double irand(int min, int max);
std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
	return std::max(lower, std::min(n, upper));
}

void convertCoord(int &x, int &y, int Xres, int Yres);
void getMouseRowCol(int &row, int &col, int mouseX, int mouseY);