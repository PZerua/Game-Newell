/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <src/input/input.h>
#include <string>

namespace gfx
{

class Window
{
private:
	// Window dimensions
	int m_width;
	int m_height;

	// Window attributes
	bool m_isFullscreen;

	GLFWwindow* m_window;

public:
	// Intializes internals
	Window();
	~Window();

	// Creates window
	bool init(const std::string &screenTitle, int window_width, int window_height, bool fullscreen = false);

	// Window dimensions
	int getWidth() const;
	int getHeight() const;

	bool isClosed() const;
	bool isFullscreen() const;
	void setSize(int width, int height);
	void toggleFullscreen();

	void update();
	void clear() const;

};

} // namespace gfx
