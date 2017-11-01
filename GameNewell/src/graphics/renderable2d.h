/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <src/math/math.h>

#include <src/graphics/texturearray.h>

namespace gfx
{

class Renderable2D
{
protected:
    math::mat4 m_model{1.0f};
    TextureArrayInfo m_textureInfo;
    GLuint m_shaderId;

public:
    Renderable2D(math::vec2 position, math::vec2 size, TextureArrayInfo textureInfo, GLuint shaderId) :
        m_textureInfo(textureInfo), m_shaderId(shaderId)
    {
        m_model.translate(math::vec3(position, 0.0f));
        m_model.scale(math::vec3(size, 1.0f));
    }

    void setTranslation(math::vec2 position) { m_model.setTranslation(position); }

    inline math::mat4 getModel() const { return m_model; }
    inline GLuint getTextureArrayId() const { return m_textureInfo.textureArrayId; }
    inline GLuint getTextureIndex() const { return m_textureInfo.textureId; }
    inline GLuint getShaderId() const { return m_shaderId; }
};

} // namespace gfx
