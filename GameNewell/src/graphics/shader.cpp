/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "shader.h"

#include <iostream>
#include <GL/glew.h>

namespace gfx
{

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// Compile shaders
	unsigned int vertex, fragment;

	compileShader(vertex, GL_VERTEX_SHADER, utils::readFile(vertexPath).c_str());
	compileShader(fragment, GL_FRAGMENT_SHADER, utils::readFile(fragmentPath).c_str());

	// Shader Program
	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	// Print errors if any
	printErrors(program);

	// Delete unnecessary shaders (we have program)
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::compileShader(unsigned int &shader, unsigned int shaderType, const char *shaderCode)
{
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	// Print compile errors if any
	printErrors(shader);
}

void Shader::printErrors(unsigned int &shader)
{
	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Error in shader compilation\n" << infoLog << std::endl;
	}
}

void Shader::enable() const
{
	glUseProgram(program);
}

void Shader::disable() const
{
	glUseProgram(0);
}

void Shader::setBool(const char* name, bool value) const
{
	glUniform1i(glGetUniformLocation(program, name), (int)value);
}
void Shader::setInt(const char* name, int value) const
{
	glUniform1i(glGetUniformLocation(program, name), value);
}
void Shader::setFloat(const char* name, float value) const
{
	glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::setVector2(const char* name, const math::vec2 &vec)
{
	glUniform2f(glGetUniformLocation(program, name), vec.x, vec.y);
}

void Shader::setVector3(const char* name, const math::vec3 &vec)
{
	glUniform3f(glGetUniformLocation(program, name), vec.x, vec.y, vec.z);
}

void Shader::setVector4(const char* name, const math::vec4 &vec)
{
	glUniform4f(glGetUniformLocation(program, name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::setMatrix4(const char* name, const math::mat4 &mat)
{
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, mat.m);
}
	
} // namespace gfx
