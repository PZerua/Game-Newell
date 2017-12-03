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
#include <src/graphics/buffers/uniformbuffer.h>
#include <src/graphics/buffers/indexbuffer.h>
#include <src/graphics/shader.h>
#include <src/graphics/uniform.h>

namespace gfx
{

#define GN_QUAD_INDICES_SIZE 6
#define GN_QUAD_VERTEX_SIZE 8

class Renderer
{

private:
    // Contains all the transformation and texture index data of a group of renderables sharing the same texture array
    struct renderableGroup
    {
        std::vector<math::mat4> transformations;
        std::vector<GLuint> textureIndices;
        // It's faster to have different vbos for each renderable group than just 2 vbos (cpu <-> gpu sync maybe?)
        VertexBuffer vboModelMatrices;
        VertexBuffer vboTextureIndices;
    };

    // Contains all the uniforms used by the shader and all the texture arrays and renderables using this shader
    struct shaderGroup
    {
        // The per shader uniforms
        std::vector<Uniform<UniformTypes>> shaderUniforms;
        // Texture array id and the renderables using that texture
        std::map<GLuint, renderableGroup> shaderRenderables;
    };

    struct
    {
        // TODO: reference this from a camera class and add a view matrix
        math::mat4 projectionMatrix;
    } m_uboData;

    // Shader id and all being rendered with it (render queue)
    std::map<GLuint, shaderGroup> m_renderQueue;

    // Map of texture arrays, they are accessed by their size (storage)
    std::map<math::vec2, std::unique_ptr<TextureArray>> m_textureArrays;

    std::unique_ptr<VertexArray> m_vao;
    std::unique_ptr<VertexBuffer> m_vbo;
    std::unique_ptr<IndexBuffer> m_ebo;
    std::unique_ptr<UniformBuffer> m_ubo;

    std::map<std::string, std::unique_ptr<Shader>> m_shaders;

    // The window we are rendering into
    std::shared_ptr<gfx::Window> m_window;

public:
    Renderer();
    void render();
    void addRenderable(const Renderable2D &sprite);
    TextureArrayInfo getTexture(const std::string &spriteName);
    GLuint getShader(const std::string &shaderName);
    void addUniform(Uniform<UniformTypes> uniform, GLint programId);
    Window *getWindow() { return m_window.get(); }
};

}
