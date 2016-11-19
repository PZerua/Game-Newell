/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "includes.h"
#include "texture.h"

class CTextureManager
{
public:
	static CTextureManager *m_instance;
	static CTextureManager *getInstance()
	{
		if (m_instance == NULL)
			m_instance = new CTextureManager();
		return m_instance;
	}
	CTexture* getTexture(const char* path);

private:
	CTextureManager();
	std::map<std::string, CTexture *> m_textureMan;
};