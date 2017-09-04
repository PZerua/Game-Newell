#pragma once

#include "includes.h"
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

class CShader
{
public:
	// Builds the shader
	CShader(const char* vertexPath, const char* fragmentPath);
	// Activate the shader
	void enable();
	void disable();
	// Utility functions
	void compileShader(unsigned int &shader, unsigned int shaderType, const char *shaderCode);
	void printErrors(unsigned int &shader);
	// Utility uniform functions
	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVector3(const char* name, const glm::vec3 &input);
	void setMatrix4(const char* name, const glm::mat4 &m);

	// Program ID
	unsigned int program;
};