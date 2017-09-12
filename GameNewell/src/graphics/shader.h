/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <src/utils/fileutils.h>
#include <src/math/math.h>

namespace gfx
{

class Shader
{
public:
	// Builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	// Activate the shader
	void enable() const;
	void disable() const;
	// Utility functions
	void compileShader(unsigned int &shader, unsigned int shaderType, const char *shaderCode);
	void printErrors(unsigned int &shader);
	// Utility uniform functions
	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVector2(const char* name, const math::vec2 &vec);
	void setVector3(const char* name, const math::vec3 &vec);
	void setVector4(const char* name, const math::vec4 &vec);
	void setMatrix4(const char* name, const math::mat4 &mat);

	// Program ID
	unsigned int program;
};

} // namespace gfx
