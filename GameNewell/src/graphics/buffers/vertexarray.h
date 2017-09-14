/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <vector>
#include <memory>

#include "src/graphics/buffers/vertexbuffer.h"
#include "src/graphics/buffers/indexbuffer.h"
#include <GL/glew.h>

namespace gfx
{

	enum VBO_FLAGS 
	{
		BUFFER_COLOR = 0x01,
		BUFFER_UV = 0x02
	};

	class VertexArray
	{
	private:
		GLuint m_vaoId;
		unsigned m_indexCount;

	public:
		VertexArray();
		~VertexArray();

		void addVertexBuffer(VertexBuffer* vertexBuffer, unsigned char flag = 0);
		void addIndexBuffer(IndexBuffer* indexBuffer);

		void bind() const;
		void unbind() const;
	};

} // namespace gfx