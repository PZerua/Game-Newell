#pragma once

#include "includes.h"
#include "texture.h"

class CTextureManager
{
public:
	static CTextureManager *instance;
	static CTextureManager *getInstance()
	{
		if (instance == NULL)
			instance = new CTextureManager();
		return instance;
	}
	CTexture* getTexture(const char* path);

private:
	CTextureManager();
	std::map<std::string, CTexture *> textureMan;
};