/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "includes.h"
#include "Texture.h"
#include "Camera.h"

class CSprite
{
public:
	CSprite();
	~CSprite();

	void render(CCamera *camera);
	void update(double deltaTime);
private:
	std::unique_ptr<CTexture> m_spriteSheet;
};