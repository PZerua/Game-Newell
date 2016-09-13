#pragma once

#include "includes.h"
#include "texture.h"

class TextureManager
{
public:
	static TextureManager *instance;
	static TextureManager *getInstance()
	{
		if (instance == NULL)
			instance = new TextureManager();
		return instance;
	}
	Texture* getTexture(const char* path);

private:
	TextureManager();
	std::map<std::string, Texture *> textureMan;
};