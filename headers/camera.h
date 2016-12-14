/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "includes.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CCamera
{
public:
	CCamera::CCamera() : scale(1), left(0), right(0), bottom(0), top(0), zNear(0), zFar(0) {}
	void translate(float x, float y);

	/**
	* Set functions
	*/
	void setPosition(glm::vec3 pos);
	void setViewport(int loc_x, int loc_y, int width, int height);
	void setOrtho(float left, float right, float bottom, float top, float zNear, float zFar, float scale);

	/**
	* Get functions
	*/
	void getViewport(int &loc_x, int &loc_y, int &width, int &height);
	void getMatricies(glm::mat4 &P, glm::mat4 &V);
	glm::vec3 getTranslation();

	int viewport_x;
	int viewport_y;
	int window_width;
	int window_height;

	double aspect;
	float scale;
	float right;
	float left;
	float bottom;
	float top;
	float zNear;
	float zFar;

	glm::vec3 camera_position;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 VP;
};
