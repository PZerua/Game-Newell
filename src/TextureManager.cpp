/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "TextureManager.h"

std::shared_ptr<CTexture> CTextureManager::getTexture(const char *path)
{
	auto pos = m_textureMan.find(std::string(path));

	if (pos == m_textureMan.end())
	{
		std::unique_ptr<CTexture> texture = std::make_unique<CTexture>();

		if (texture->load(path))
			m_textureMan[std::string(path)] = std::move(texture);
		else
		{
			std::cout << "Texture " << path << " not found" << std::endl;
			return nullptr;
		}
	}

	return m_textureMan[std::string(path)];
}