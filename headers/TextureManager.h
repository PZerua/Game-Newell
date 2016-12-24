/**
* (c) 2016 Pablo Luis Garc�a. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "includes.h"
#include "texture.h"

class CTextureManager
{
public:
	static CTextureManager& getInstance()
	{
		static CTextureManager singleton;
		return singleton;
	}
	std::shared_ptr<CTexture> getTexture(const char* path);

private:
	CTextureManager() {};
	std::map<std::string, std::shared_ptr<CTexture>> m_textureMan;
};
