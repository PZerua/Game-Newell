/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "Window.h"
#include <iostream>

CWindow::CWindow() :
	mWindow(NULL),
	_pressing(false),
	_isFullscreen(false),
	mMouseFocus(false),
	mKeyboardFocus(false),
	mFullScreen(false),
	mShown(false),
	mClosed(false),
	mWindowID(-1),
	mWidth(0),
	mHeight(0)
{}

CWindow::~CWindow()
{
	if (mWindow != NULL)
	{
		SDL_DestroyWindow(mWindow);
	}
}

SDL_Window* CWindow::init(const std::string &screenTitle, int window_width, int window_height, bool fullscreen)
{
	// Set attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Create window
	mWindow = SDL_CreateWindow(screenTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL
		| SDL_WINDOW_RESIZABLE
		| (fullscreen ? SDL_WINDOW_FULLSCREEN : 0));

	std::cout << "[GAME] Window created" << std::endl;
	if (mWindow != NULL)
	{
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = window_width;
		mHeight = window_height;

		// Create renderer for window
		SDL_GLContext glcontext = SDL_GL_CreateContext(mWindow);

		std::cout << "[GAME] OpenGL context created" << std::endl;

		atexit(SDL_Quit);

		// Grab window identifier
		mWindowID = SDL_GetWindowID(mWindow);

		// Flag as opened
		mShown = true;
	}
	else
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	}

#ifdef USE_GLEW
	glewInit();
#endif

	return mWindow;
}

void CWindow::handleEvent(SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_F11:
			if (!_isFullscreen && !_pressing)
			{
				SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
				_isFullscreen = true;
				_pressing = true;
			}
			else if (_isFullscreen && !_pressing)
			{
				SDL_SetWindowFullscreen(mWindow, 0);
				_isFullscreen = false;
				_pressing = true;
			}
			break;
		}
		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_F11:
			_pressing = false;
			break;
		}
		break;
	}
}

void CWindow::focus()
{
	// Restore window if needed
	if (!mShown)
	{
		SDL_ShowWindow(mWindow);
	}

	// Move window forward
	SDL_RaiseWindow(mWindow);
}

void CWindow::free()
{
	if (mWindow != NULL)
	{
		SDL_DestroyWindow(mWindow);
	}

	mMouseFocus = false;
	mKeyboardFocus = false;
	mWidth = 0;
	mHeight = 0;
}

int CWindow::getWidth()
{
	return mWidth;
}

int CWindow::getHeight()
{
	return mHeight;
}

bool CWindow::hasMouseFocus()
{
	return mMouseFocus;
}

bool CWindow::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

bool CWindow::isMinimized()
{
	return mMinimized;
}

bool CWindow::isShown()
{
	return mShown;
}

bool CWindow::isClosed()
{
	return mClosed;
}

void CWindow::setFocus(bool focus)
{
	mMouseFocus = focus;
}

bool CWindow::isFullscreen()
{
	return _isFullscreen;
}

void CWindow::setSize(int width, int height)
{
	mWidth = width;
	mHeight = height;

	SDL_SetWindowSize(mWindow, width, height);
	SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void CWindow::setFullscreen()
{
	SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
}