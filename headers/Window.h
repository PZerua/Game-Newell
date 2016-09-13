#pragma once

#include "includes.h"
#include <string>

class Window
{
public:
	//Intializes internals
	Window();

	//Creates window
	SDL_Window* init(const std::string &screenTitle, int window_width, int window_height, bool fullscreen = false);

	//Handles window events
	void handleEvent(SDL_Event& e);

	//Focuses on window
	void focus();

	//Print render
	void Present();

	//Clear screen
	void Clear();

	//Deallocates internals
	void free();

	void setFocus(bool focus);

	//Window dimensions
	int getWidth();
	int getHeight();

	//Window focus
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();
	bool isShown();
	bool isClosed();
	bool isFullscreen();
	void setSize(int width, int height);

	SDL_Window* mWindow;

	//Window dimensions
	int mWidth;
	int mHeight;

private:
	//Window data
	int mWindowID;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
	bool mShown;
	bool mClosed;
	bool _isFullscreen;
	bool _pressing;
};
