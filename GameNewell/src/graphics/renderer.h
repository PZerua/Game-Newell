/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <vector>
#include <map>

#include <src/graphics/window.h>
#include <src/graphics/renderable2d.h>
#include <src/graphics/texturearray.h>
#include <src/graphics/buffers/vertexarray.h>
#include <src/graphics/buffers/vertexbuffer.h>
#include <src/graphics/buffers/indexbuffer.h>
#include <src/graphics/shader.h>

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
        // It's faster to have different vbos for each renderable group than just 2 vbos (cpu <-> gpu sync maybe?)
        VertexBuffer vboModelMatrices;
        VertexBuffer vboTextureIndices;
    };

    // Shader id - Texture array id and the renderables using that texture (render queue)
    std::map<GLuint, std::map<GLuint, renderableGroup>> m_renderQueue;

    // Map of texture arrays, they are accessed by their size (storage)
    std::map<math::vec2, std::unique_ptr<TextureArray>> m_textureArrays;

    std::unique_ptr<VertexArray> m_vao;
    std::unique_ptr<VertexBuffer> m_vbo;
    std::unique_ptr<IndexBuffer> m_ebo;

    // The window we are rendering into
    std::shared_ptr<gfx::Window> m_window;

    math::mat4 m_projectionMatrix;

public:
    Renderer();
    void render();
    void addRenderable(Renderable2D &sprite);
    TextureArrayInfo getTexture(const std::string &spriteName);
    Window *getWindow() { return m_window.get(); }
};

}
