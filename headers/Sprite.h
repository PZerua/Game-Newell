/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "includes.h"
#include "Texture.h"
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"

class CSprite
{
public:
	CSprite(std::string spriteSheet);
	~CSprite();

	void render(CCamera *camera);
	void update(double deltaTime);
private:
	std::shared_ptr<CTexture> m_spriteSheet;
	std::shared_ptr<CShader> m_spriteShader;
	glm::mat4 m_modelMatrix;;

	CMesh m_quad;
};