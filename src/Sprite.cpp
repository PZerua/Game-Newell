#include "Sprite.h"
#include "TextureManager.h"

CSprite::CSprite(std::string spriteSheet)
{
	m_spriteShader = std::make_shared<CShader>("data/shaders/simple.vs", "data/shaders/simple.fs");
	m_spriteSheet = CTextureManager::getInstance().getTexture(spriteSheet.c_str());
	m_quad.createQuad(SPRITE_WIDTH * 0.5, SPRITE_HEIGHT * 0.5, SPRITE_WIDTH, SPRITE_HEIGHT, 0, 0, SPRITESHEET_SIZE);
}

CSprite::~CSprite()
{
}

void CSprite::render(CCamera *camera)
{
	m_spriteShader->enable();
	
	glBindTexture(GL_TEXTURE_2D, m_spriteSheet->m_texture_id);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_spriteShader->setMatrix4("u_mvp", camera->VP * m_modelMatrix);
	m_quad.render(GL_TRIANGLES, m_spriteShader.get());

	glDisable(GL_BLEND);
}

void CSprite::update(double deltaTime)
{
}