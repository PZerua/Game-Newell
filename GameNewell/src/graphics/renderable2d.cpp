/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "renderable2d.h"

namespace gfx
{
	std::unique_ptr<IndexBuffer> Renderable2D::m_ebo = nullptr;

	Renderable2D::Renderable2D(math::vec2 size) :
		m_size(size)
	{
		initEbo();
		initVbo();
	}


	Renderable2D::Renderable2D(math::vec2 size, math::vec4 color) :
		m_size(size), m_color(color)
	{
		initEbo();
		initVbo(BUFFER_COLOR);
	}

	void Renderable2D::initVao(unsigned char flag)
	{
		
		m_vao = std::make_unique<VertexArray>();
		m_vao->addVertexBuffer(m_vbo.get());

	}

	void Renderable2D::initEbo()
	{
		if (!m_ebo)
		{
			GLushort indices[QUAD_INDICES_SIZE] = { 0, 1, 2, 1, 3, 2 };
			m_ebo = std::make_unique<IndexBuffer>(indices, QUAD_INDICES_SIZE);
		}
	}

	void Renderable2D::initVbo(unsigned char flag)
	{
		float center_x = m_size.x * 0.5f;
		float center_y = m_size.y * 0.5f;

		bool hasColor = flag & BUFFER_COLOR;

		if (hasColor)
		{
			GLfloat vertexData[QUAD_VERTEXCOLOR_SIZE] =
			{
				center_x - m_size.x * 0.5f, center_y - m_size.y * 0.5f, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w,
				center_x + m_size.x * 0.5f, center_y - m_size.y * 0.5f, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w,
				center_x - m_size.x * 0.5f, center_y + m_size.y * 0.5f, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w,
				center_x + m_size.x * 0.5f, center_y + m_size.y * 0.5f, 0.0f, m_color.x, m_color.y, m_color.z, m_color.w
			};
		}

		GLfloat vertexData[QUAD_VERTEX_SIZE] =
		{
			center_x - m_size.x * 0.5f, center_y - m_size.y * 0.5f, 0.0f,
			center_x + m_size.x * 0.5f, center_y - m_size.y * 0.5f, 0.0f,
			center_x - m_size.x * 0.5f, center_y + m_size.y * 0.5f, 0.0f,
			center_x + m_size.x * 0.5f, center_y + m_size.y * 0.5f, 0.0f
		};

		m_vbo = std::make_unique<VertexBuffer>(vertexData, hasColor ? QUAD_VERTEXCOLOR_SIZE : QUAD_VERTEX_SIZE, hasColor ? 7 : 3);
	}

} // namespace gfx