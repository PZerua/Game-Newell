#include "TextureManager.h"

CTextureManager *CTextureManager::instance = NULL;

CTextureManager::CTextureManager()
{
	if (instance == NULL)
		instance = this;
}

CTexture *CTextureManager::getTexture(const char *path)
{
	auto pos = textureMan.find(std::string(path));

	CTexture *texture = new CTexture();
	if (pos == textureMan.end())
	{
		if (texture->load(path))
			textureMan[std::string(path)] = texture;
		else
		{
			std::cout << "Texture " << path << " not found" << std::endl;
			delete texture;
			return NULL;
		}
	}

	return textureMan[std::string(path)];
}