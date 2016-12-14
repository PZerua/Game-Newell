/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "camera.h"
#include <glm\gtx\string_cast.hpp>

void CCamera::setOrtho(float left, float right, float bottom, float top, float zNear, float zFar, float scale)
{
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	this->zNear = zNear;
	this->zFar = zFar;
	this->scale = scale;

	projection = glm::ortho(left * scale, right * scale, bottom * scale, top * scale, zNear, zFar);
	VP = projection * view;
}

void CCamera::setPosition(glm::vec3 pos)
{
	view = glm::translate(glm::mat4(1.0f), pos);
	VP = projection * view;
}

void CCamera::setViewport(int loc_x, int loc_y, int width, int height)
{
	viewport_x = loc_x;
	viewport_y = loc_y;
	window_width = width;
	window_height = height;

	aspect = double(width) / double(height);
	glViewport(viewport_x, viewport_y, window_width, window_height);
}

void CCamera::getViewport(int &loc_x, int &loc_y, int &width, int &height)
{
	loc_x = viewport_x;
	loc_y = viewport_y;
	width = window_width;
	height = window_height;
}

void CCamera::getMatricies(glm::mat4 &P, glm::mat4 &V)
{
	P = projection;
	V = view;
}

glm::vec3 CCamera::getTranslation()
{
	return glm::vec3(view[3]);
}

void CCamera::translate(float x, float y)
{
	view = glm::translate(view, glm::vec3(x, y, 0.0f));
	//std::cout << glm::to_string(view) << std::endl << std::endl;
	VP = projection * view;
}