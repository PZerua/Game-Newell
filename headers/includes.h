/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#define USE_GLEW
#include <GL/glew.h>

#define BASE_RESOLUTION_WIDTH 160.0f
#define BASE_RESOLUTION_HEIGHT 144.0f
#define TILE_SIZE 16.0f
#define SPRITE_WIDTH 8.0f
#define SPRITE_HEIGHT 16.0f
#define TILEMAP_SIZE 8.0f
#define SPRITESHEET_SIZE 4.0f

// SDL
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include <iostream>
#include <memory>