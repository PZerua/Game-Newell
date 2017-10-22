/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <vector>
#include <map>

#include <src/graphics/sprite.h>
#include <src/graphics/textureManager.h>
#include <src/graphics/buffers/vertexarray.h>
#include <src/graphics/buffers/vertexbuffer.h>
#include <src/graphics/buffers/indexbuffer.h>

namespace gfx
{

#define GN_QUAD_INDICES_SIZE 6
#define GN_QUAD_VERTEX_SIZE 8
#define GN_QUAD_VERTEXUV_SIZE GN_QUAD_VERTEX_SIZE * 2

class Renderer
{

private:

	struct renderableGroup
	{
		std::vector<math::mat4> transformations;
		std::vector<GLuint> textureIndices;
		std::unique_ptr<VertexBuffer> vbo_modelMatrices;
		std::unique_ptr<VertexBuffer> vbo_textureIndices;
	};

	// Texture array id and the number of renderables
	std::map<GLuint, renderableGroup> m_renderables;

	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<VertexBuffer> m_vbo;
	std::unique_ptr<IndexBuffer> m_ebo;

	TextureManager m_manager;

public:
	Renderer();
	void render();
	void addRenderable(Sprite &sprite);
	void initRenderableGroup(GLuint id);
	std::pair<GLuint, GLuint> getTexture(const char *fileName);
};

}
