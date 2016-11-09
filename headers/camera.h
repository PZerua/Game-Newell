#ifndef CAMERA_H
#define CAMERA_H

#include "includes.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CCamera {
	public:
		CCamera::CCamera() : scale(1), left(0), right(0), bottom(0), top(0), zNear(0), zFar(0) {}

		// Setting Functions
		// Set the position of the camera
		void setPosition(glm::vec3 pos);
		// Change the viewport location and size
		void setViewport(int loc_x, int loc_y, int width, int height);
		void translate(float x, float y);
		void setOrtho(float left, float right, float bottom, float top, float zNear, float zFar, float scale);

		// Getting Functions
		void getViewport(int &loc_x, int &loc_y, int &width, int &height);
		void getMatricies(glm::mat4 &P, glm::mat4 &V);

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
#endif