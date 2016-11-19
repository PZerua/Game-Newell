/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "TextureManager.h"

CTextureManager *CTextureManager::m_instance = NULL;

CTextureManager::CTextureManager()
{
	if (m_instance == NULL)
		m_instance = this;
}

CTexture *CTextureManager::getTexture(const char *path)
{
	auto pos = m_textureMan.find(std::string(path));

	CTexture *texture = new CTexture();
	if (pos == m_textureMan.end())
	{
		if (texture->load(path))
			m_textureMan[std::string(path)] = texture;
		else
		{
			std::cout << "Texture " << path << " not found" << std::endl;
			delete texture;
			return NULL;
		}
	}

	return m_textureMan[std::string(path)];
}