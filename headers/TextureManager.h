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