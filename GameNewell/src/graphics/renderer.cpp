/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#include "renderer.h"

namespace gfx
{

Renderer::Renderer()
{
    m_window = std::make_shared<gfx::Window>();
    m_window->init("Game Newell", 1280, 720);
    m_window->setClearColor(225, 208, 130);

    m_projectionMatrix = math::mat4::ortho(0.0f, (float)m_window->getWidth(), (float)m_window->getHeight(), 0.0f, -1.0f, 1.0f);

    m_vao = std::make_unique<VertexArray>();

    // -- Setup indices --
    GLubyte indices[GN_QUAD_INDICES_SIZE] = { 0, 1, 2, 1, 3, 2 };
    m_ebo = std::make_unique<IndexBuffer>(indices, (GLsizei)(GN_QUAD_INDICES_SIZE * sizeof(GLubyte)));

    // -- Setup vertex data --
    GLfloat vertexData[GN_QUAD_VERTEXUV_SIZE] =
    {
        // Vertex   // Uv
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    // Vbo for the indexed quad
    m_vbo = std::make_unique<VertexBuffer>(vertexData, (GLsizei)(GN_QUAD_VERTEXUV_SIZE * sizeof(GLfloat)));

    unsigned stride = 4; // (2 + 2) Vertex + uv

    // Vertex data
    glEnableVertexAttribArray(ATTRIBUTE_VERTEX);
    glVertexAttribPointer(ATTRIBUTE_VERTEX, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)0);

    // Uv data
    glEnableVertexAttribArray(ATTRIBUTE_UV);
    glVertexAttribPointer(ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    // The model matrices are uploaded as four vec4, the format is specified here and the vbo is bound later in the render pass
    glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_MODELMATRIX + 0);
    glVertexAttribFormat(ATTRIBUTE_INSTANCE_MODELMATRIX + 0, 4, GL_FLOAT, GL_FALSE, 0);

    glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_MODELMATRIX + 1);
    glVertexAttribFormat(ATTRIBUTE_INSTANCE_MODELMATRIX + 1, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(math::vec4));

    glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_MODELMATRIX + 2);
    glVertexAttribFormat(ATTRIBUTE_INSTANCE_MODELMATRIX + 2, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(math::vec4));

    glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_MODELMATRIX + 3);
    glVertexAttribFormat(ATTRIBUTE_INSTANCE_MODELMATRIX + 3, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(math::vec4));

    // Bind to third vbo (third position in shader attributes)
    glVertexAttribBinding(ATTRIBUTE_INSTANCE_MODELMATRIX + 0, ATTRIBUTE_INSTANCE_MODELMATRIX);
    glVertexAttribBinding(ATTRIBUTE_INSTANCE_MODELMATRIX + 1, ATTRIBUTE_INSTANCE_MODELMATRIX);
    glVertexAttribBinding(ATTRIBUTE_INSTANCE_MODELMATRIX + 2, ATTRIBUTE_INSTANCE_MODELMATRIX);
    glVertexAttribBinding(ATTRIBUTE_INSTANCE_MODELMATRIX + 3, ATTRIBUTE_INSTANCE_MODELMATRIX);

    // Get next attribute every instance (and not every vertex)
    glVertexBindingDivisor(ATTRIBUTE_INSTANCE_MODELMATRIX, 1);

    // The texture indices are uploaded as a GLuint, the format is specified here and the vbo is bound later in the render pass
    glEnableVertexAttribArray(ATTRIBUTE_INSTANCE_TEXTUREINDICES);
    glVertexAttribFormat(ATTRIBUTE_INSTANCE_TEXTUREINDICES, 1, GL_FLOAT, GL_FALSE, 0);

    // Bint to seventh vbo (seventh position in shader attributes)
    glVertexAttribBinding(ATTRIBUTE_INSTANCE_TEXTUREINDICES, ATTRIBUTE_INSTANCE_TEXTUREINDICES);

    m_vao->unbind();
}

TextureArrayInfo Renderer::getTexture(const std::string &spriteName)
{
    std::string spritePath = "data/sprites/" + spriteName + ".png";
    math::vec2 size;

    if (utils::getTextureSize(spritePath.c_str(), size))
    {
        // Check if texture array already exists
        if (!m_textureArrays.count(size))
            m_textureArrays[size] = std::make_unique<TextureArray>((unsigned)size.x, (unsigned)size.y);
    }

    return { m_textureArrays[size]->getId(), m_textureArrays[size]->getTexture(spritePath.c_str()) };
}

void Renderer::addRenderable(Renderable2D &renderable)
{
    auto &textureGroups = m_renderQueue[renderable.getShaderId()];
    auto &group = textureGroups[renderable.getTextureArrayId()];

    // Add the sprite info to the corresponding renderable group (render queue)
    group.transformations.push_back(renderable.getModel());
    group.textureIndices.push_back(renderable.getTextureIndex());
}

void Renderer::render()
{
    m_vao->bind();

    for (auto &textureGroups : m_renderQueue)
    {
        // Bind the shader
        glUseProgram(textureGroups.first);

        // Set projection matrix
        glUniformMatrix4fv(glGetUniformLocation(textureGroups.first, "uProjection"), 1, GL_FALSE, m_projectionMatrix.m);

        for (auto &group : textureGroups.second)
        {
            // Skip if the group is empty
            if (!group.second.transformations.size())
                continue;

            // Setup the vbos for each renderable group
            group.second.vboModelMatrices.changeData(group.second.transformations.data(), (GLsizei)(group.second.transformations.size() * sizeof(math::mat4)));
            group.second.vboTextureIndices.changeData(group.second.textureIndices.data(), (GLsizei)(group.second.textureIndices.size() * sizeof(GLuint)));

            // Bind current texture array
            glBindTexture(GL_TEXTURE_2D_ARRAY, group.first);
            // Bind model matrices and texture indices vbos
            glBindVertexBuffer(ATTRIBUTE_INSTANCE_MODELMATRIX, group.second.vboModelMatrices.getId(), 0, 4 * sizeof(math::vec4));
            glBindVertexBuffer(ATTRIBUTE_INSTANCE_TEXTUREINDICES, group.second.vboTextureIndices.getId(), 0, sizeof(GLuint));
            // Draw instanced quads
            glDrawElementsInstanced(GL_TRIANGLES, GN_QUAD_INDICES_SIZE, GL_UNSIGNED_BYTE, NULL, (GLsizei)group.second.transformations.size());

            // Since the vectors depend on the current renderables to render, they need to be cleared for the next render pass
            group.second.transformations.clear();
            group.second.textureIndices.clear();
        }
    }

    m_vao->unbind();
}

}
