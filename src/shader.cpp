#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

CShader::CShader(const char* vertexPath, const char* fragmentPath)
{
	// Retrieve the vertex/fragment source code from filePath
	std::ifstream vShaderFile, fShaderFile;
	std::stringstream vShaderStream, fShaderStream;

	// Open files
	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);
	// Read from buffer
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	// Close files
	vShaderFile.close();
	fShaderFile.close();

	// Compile shaders
	unsigned int vertex, fragment;

	compileShader(vertex, GL_VERTEX_SHADER, vShaderStream.str().c_str());
	compileShader(fragment, GL_FRAGMENT_SHADER, fShaderStream.str().c_str());

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

void CShader::compileShader(unsigned int &shader, unsigned int shaderType, const char *shaderCode)
{
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	// Print compile errors if any
	printErrors(shader);
}

void CShader::printErrors(unsigned int &shader)
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

void CShader::enable()
{
	glUseProgram(program);
}

void CShader::disable()
{
	glUseProgram(0);
}


void CShader::setBool(const char* name, bool value) const
{
	glUniform1i(glGetUniformLocation(program, name), (int)value);
}
void CShader::setInt(const char* name, int value) const
{
	glUniform1i(glGetUniformLocation(program, name), value);
}
void CShader::setFloat(const char* name, float value) const
{
	glUniform1f(glGetUniformLocation(program, name), value);
}

void CShader::setVector3(const char* name, const glm::vec3 &input)
{
	glUniform3f(glGetUniformLocation(program, name), input.x, input.y, input.z);
}

void CShader::setMatrix4(const char* name, const glm::mat4 &m)
{
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(m));
}