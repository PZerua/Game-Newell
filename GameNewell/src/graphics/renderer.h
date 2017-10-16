/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <vector>

#include <src/graphics/sprite.h>
#include <src/graphics/buffers/vertexarray.h>
#include <src/graphics/buffers/vertexbuffer.h>
#include <src/graphics/buffers/indexbuffer.h>

namespace gfx
{

#define GN_QUAD_INDICES_SIZE 6
#define GN_QUAD_VERTEX_SIZE 8
#define GN_QUAD_VERTEXUV_SIZE 16

class Renderer
{

private:
	// Supposed to be sorted before added
	std::vector<math::mat4> m_transformations;
	std::vector<GLuint> m_textureIndices;

	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<VertexBuffer> m_vbo;
	std::unique_ptr<VertexBuffer> m_vbo_instancedModel;
	std::unique_ptr<VertexBuffer> m_vbo_instancedTextureIndices;
	std::unique_ptr<IndexBuffer> m_ebo;

	unsigned char m_flags;

public:
	Renderer(unsigned char flags);
	void render();
	void addRenderable(Sprite *sprite);

private:
	void initVao();
	void initVbo();
	void initEbo();
	
};

}
