/**
* (c) 2017 Pablo Luis Garc�a. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <vector>
#include <memory>

#include "src/graphics/buffers/vertexbuffer.h"
#include "src/graphics/buffers/indexbuffer.h"
#include <GL/glew.h>

namespace gfx
{

#define ATTRIBUTE_VERTEX 0
#define ATTRIBUTE_COLOR 1
#define ATTRIBUTE_UV 2
#define ATTRIBUTE_INSTANCE_MODELMATRIX 3
#define ATTRIBUTE_INSTANCE_TEXTUREINDICES 7

class VertexArray
{
private:
    GLuint m_vaoId;

public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;
};

} // namespace gfx
