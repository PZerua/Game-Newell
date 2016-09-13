/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This contains several functions that can be useful when programming your game.
*/
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <vector>

#include "framework.h"

//General functions **************

//generic purposes fuctions
void drawGrid(float dist);

//check opengl errors
bool checkGLErrors();

Vector2 getDesktopSize(int display_index = 0);

double irand(int min, int max);
void convertCoord(int &x, int &y, int Xres, int Yres);
void getMouseRowCol(int &row, int &col, int mouseX, int mouseY);

std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);


#endif
