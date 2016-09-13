#include "Window.h"
#include <iostream>

Window::Window() :
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

SDL_Window* Window::init(const std::string &screenTitle, int window_width, int window_height, bool fullscreen)
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

void Window::handleEvent(SDL_Event& e)
{
	// If an event was detected for this window
	if (e.type == (unsigned)SDL_WINDOWEVENT)
	{
		switch (e.window.event)
		{
			// Window appeared
		case SDL_WINDOWEVENT_SHOWN:
			mShown = true;
			break;

			// Window disappeared
		case SDL_WINDOWEVENT_HIDDEN:
			mShown = false;
			break;

			// Get new dimensions and repaint
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			// SDL_RenderPresent(mRenderer);
			break;

			// Repaint on expose
		case SDL_WINDOWEVENT_EXPOSED:
			// SDL_RenderPresent(mRenderer);
			break;

			// Mouse enter
		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			break;

			// Mouse exit
		case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			break;

			// Keyboard focus gained
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			break;

			// Keyboard focus lost
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			break;

			// Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			mMinimized = true;
			break;

			// Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
			break;

			// Window restored
		case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;
			break;

			// Hide on close
		case SDL_WINDOWEVENT_CLOSE:
			free();
			mClosed = true;
			break;
		}
	}
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

void Window::focus()
{
	// Restore window if needed
	if (!mShown)
	{
		SDL_ShowWindow(mWindow);
	}

	// Move window forward
	SDL_RaiseWindow(mWindow);
}

void Window::Present()
{
	if (!mMinimized)
	{
		//Update screen
		//SDL_RenderPresent(mRenderer);
	}
}

void Window::Clear()
{
	if (!mMinimized)
	{
		//Clear screen
		//SDL_RenderClear(mRenderer);
	}
}

void Window::free()
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

int Window::getWidth()
{
	return mWidth;
}

int Window::getHeight()
{
	return mHeight;
}

bool Window::hasMouseFocus()
{
	return mMouseFocus;
}

bool Window::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

bool Window::isMinimized()
{
	return mMinimized;
}

bool Window::isShown()
{
	return mShown;
}

bool Window::isClosed()
{
	return mClosed;
}

void Window::setFocus(bool focus)
{
	mMouseFocus = focus;
}

bool Window::isFullscreen()
{
	return _isFullscreen;
}

void Window::setSize(int width, int height)
{
	mWidth = width;
	mHeight = height;

	SDL_SetWindowSize(mWindow, width, height);
	SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}
