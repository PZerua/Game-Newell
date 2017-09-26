/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <src/utils/fileutils.h>
#include <src/math/math.h>
#include <GL/glew.h>
#include <src/graphics/textureArray.h>

namespace gfx
{

class Shader
{
private:
	// Program ID
	GLuint m_programID;

public:
	// Builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	// Activate the shader
	void enable() const;
	void disable() const;
	// Utility uniform functions
	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVector2(const char* name, const math::vec2 &vec) const;
	void setVector3(const char* name, const math::vec3 &vec) const;
	void setVector4(const char* name, const math::vec4 &vec) const;
	void setMatrix4(const char* name, const math::mat4 &mat) const;
	void setTextureArray(const char* name, const TextureArray &tex, GLenum target) const;

private:
	// Utility functions
	void compileShader(GLuint &shader, GLenum shaderType, const char *shaderCode);
	void printShaderErrors(GLuint shader) const;
	void printProgramErrors(GLuint shader) const;
};

} // namespace gfx
