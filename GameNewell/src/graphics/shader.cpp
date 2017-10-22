/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#include "shader.h"

#include <iostream>

namespace gfx
{

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // Compile shaders
    GLuint vertex, fragment;

    compileShader(vertex, GL_VERTEX_SHADER, utils::readFileString(vertexPath).c_str());
    compileShader(fragment, GL_FRAGMENT_SHADER, utils::readFileString(fragmentPath).c_str());

    // Shader Program
    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertex);
    glAttachShader(m_programID, fragment);
    glLinkProgram(m_programID);

    // Print errors if any
    printProgramErrors(m_programID);

    // Delete unnecessary shaders (we have program)
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(m_programID);
}

void Shader::compileShader(GLuint &shader, GLenum shaderType, const char *shaderCode)
{
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    // Print compile errors if any
    printShaderErrors(shader);
}

void Shader::printShaderErrors(GLuint shader) const
{
    GLint success;
    char compileLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, compileLog);
        std::cout << "Error in shader compilation\n" << compileLog << std::endl;
    }
}

void Shader::printProgramErrors(GLuint shader) const
{
    GLint success;
    char linkLog[512];

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader, 512, NULL, linkLog);
        std::cout << "Error in shader compilation\n" << linkLog << std::endl;
    }
}

void Shader::enable() const
{
    glUseProgram(m_programID);
}

void Shader::disable() const
{
    glUseProgram(0);
}

void Shader::setBool(const char* name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_programID, name), (int)value);
}
void Shader::setInt(const char* name, int value) const
{
    glUniform1i(glGetUniformLocation(m_programID, name), value);
}
void Shader::setFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(m_programID, name), value);
}

void Shader::setVector2(const char* name, const math::vec2 &vec) const
{
    glUniform2f(glGetUniformLocation(m_programID, name), vec.x, vec.y);
}

void Shader::setVector3(const char* name, const math::vec3 &vec) const
{
    glUniform3f(glGetUniformLocation(m_programID, name), vec.x, vec.y, vec.z);
}

void Shader::setVector4(const char* name, const math::vec4 &vec) const
{
    glUniform4f(glGetUniformLocation(m_programID, name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::setMatrix4(const char* name, const math::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_programID, name), 1, GL_FALSE, mat.m);
}

void Shader::setTextureArray(const char* name, const TextureArray &tex) const
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex.getId());
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(m_programID, name), tex.getTextureCount());
}

} // namespace gfx
