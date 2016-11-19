/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This allows to use compile and use shaders when rendering. Used for advanced lighting.
*/

#include "shader.h"
#include <cassert>
#include <vector>
#include "texture.h"
#include "utils.h"
#include <glm/gtc/type_ptr.hpp>

std::map<std::string, CShader*> CShader::s_Shaders;
bool CShader::s_ready = false;

CShader::CShader()
{
	if (!CShader::s_ready)
		CShader::init();
	compiled = false;
}

CShader::~CShader()
{
	release();
}

void CShader::setFilenames(const std::string& vsf, const std::string& psf)
{
	vs_filename = vsf;
	ps_filename = psf;
}

bool CShader::load(const std::string& vsf, const std::string& psf)
{
	assert(compiled == false);
	assert(glGetError() == GL_NO_ERROR);

	vs_filename = vsf;
	ps_filename = psf;

	std::cout << " * Shader: Vertex: " << vsf << "  Pixel: " << psf << std::endl;
	std::string vsm, psm;
	if (!readFile(vsf, vsm) || !readFile(psf, psm))
		return false;

	//printf("Vertex shader from memory:\n%s\n", vsm.c_str());
	//printf("Fragment shader from memory:\n%s\n", psm.c_str());

	if (!compileFromMemory(vsm, psm))
		return false;

	assert(glGetError() == GL_NO_ERROR);

	return true;
}

CShader* CShader::Load(const char* vsf, const char* psf)
{
	std::string name = std::string(vsf) + "," + std::string(psf);
	std::map<std::string, CShader*>::iterator it = s_Shaders.find(name);
	if (it != s_Shaders.end())
		return it->second;

	CShader* sh = new CShader();
	if (!sh->load(vsf, psf))
		return NULL;
	s_Shaders[name] = sh;
	return sh;
}

void CShader::ReloadAll()
{
	for (std::map<std::string, CShader*>::iterator it = s_Shaders.begin(); it != s_Shaders.end(); it++)
		it->second->compile();
	std::cout << "Shaders recompiled" << std::endl;
}

bool CShader::compile()
{
	assert(!compiled && "Shader already compiled");
	return load(vs_filename, ps_filename);
}

bool CShader::recompile()
{
	release(); //remove old shader
	return load(vs_filename, ps_filename);
}

bool CShader::readFile(const std::string& filename, std::string& content)
{
	content.clear();

	long count = 0;

	FILE *fp;
	fopen_s(&fp, filename.c_str(), "rb");

	if (fp == NULL)
	{
		std::cerr << "Shader::readFile: file not found " << filename << std::endl;
		return false;
	}

	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	content.resize(count);
	if (count > 0)
	{
		count = fread(&content[0], sizeof(char), count, fp);
	}
	fclose(fp);

	return true;
}

std::string CShader::getInfoLog() const
{
	return info_log;
}

bool CShader::hasInfoLog() const
{
	return info_log.size() > 0;
}

// ******************************************

bool CShader::compileFromMemory(const std::string& vsm, const std::string& psm)
{
	if (glCreateProgram == 0)
	{
		std::cout << "Error: your graphics cards dont support shaders. Sorry." << std::endl;
		exit(0);
	}

	program = glCreateProgram();
	assert(glGetError() == GL_NO_ERROR);

	if (!createVertexShaderObject(vsm))
	{
		printf("Vertex shader compilation failed\n");
		return false;
	}

	if (!createFragmentShaderObject(psm))
	{
		printf("Fragment shader compilation failed\n");
		return false;
	}

	glLinkProgram(program);
	assert(glGetError() == GL_NO_ERROR);

	GLint linked = 0;

	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	assert(glGetError() == GL_NO_ERROR);

	if (!linked)
	{
		saveProgramInfoLog(program);
		release();
		return false;
	}

#ifdef _DEBUG
	validate();
#endif

	compiled = true;

	return true;
}

bool CShader::validate()
{
	glValidateProgram(program);
	assert(glGetError() == GL_NO_ERROR);

	GLint validated = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &validated);
	assert(glGetError() == GL_NO_ERROR);

	if (!validated)
	{
		printf("Shader validation failed\n");
		saveProgramInfoLog(program);
		return false;
	}

	return true;
}

bool CShader::createVertexShaderObject(const std::string& shader)
{
	return createShaderObject(GL_VERTEX_SHADER, vs, shader);
}

bool CShader::createFragmentShaderObject(const std::string& shader)
{
	return createShaderObject(GL_FRAGMENT_SHADER, fs, shader);
}

bool CShader::createShaderObject(unsigned int type, GLuint& handle, const std::string& code)
{
	handle = glCreateShader(type);
	assert(glGetError() == GL_NO_ERROR);

	std::string prefix = "#define DESKTOP\n";

	std::string fullcode = prefix + code;
	const char* ptr = fullcode.c_str();
	glShaderSource(handle, 1, &ptr, NULL);
	assert(glGetError() == GL_NO_ERROR);

	glCompileShader(handle);
	assert(glGetError() == GL_NO_ERROR);

	GLint compile = 0;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &compile);
	assert(glGetError() == GL_NO_ERROR);

	//we want to see the compile log if we are in debug (to check warnings)
	if (!compile)
	{
		saveShaderInfoLog(handle);
		std::cout << "Shader code:\n " << std::endl;
		std::vector<std::string> lines = split(fullcode, '\n');
		for (size_t i = 0; i < lines.size(); ++i)
			std::cout << i << "  " << lines[i] << std::endl;

		return false;
	}

	glAttachShader(program, handle);
	assert(glGetError() == GL_NO_ERROR);

	return true;
}

void CShader::release()
{
	if (vs)
	{
		glDeleteShader(vs);
		assert(glGetError() == GL_NO_ERROR);
		vs = 0;
	}

	if (fs)
	{
		glDeleteShader(fs);
		assert(glGetError() == GL_NO_ERROR);
		fs = 0;
	}

	if (program)
	{
		glDeleteProgram(program);
		assert(glGetError() == GL_NO_ERROR);
		program = 0;
	}

	compiled = false;
}

void CShader::enable()
{
	glUseProgram(program);
	assert(glGetError() == GL_NO_ERROR);

	last_slot = 0;
}

void CShader::disable()
{
	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::disableShaders()
{
	glUseProgram(0);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::saveShaderInfoLog(GLuint obj)
{
	int len = 0;
	assert(glGetError() == GL_NO_ERROR);
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &len);
	assert(glGetError() == GL_NO_ERROR);

	if (len > 0)
	{
		char* ptr = new char[len + 1];
		GLsizei written = 0;
		glGetShaderInfoLog(obj, len, &written, ptr);
		ptr[written - 1] = '\0';
		assert(glGetError() == GL_NO_ERROR);
		log.append(ptr);
		delete[] ptr;

		printf("LOG **********************************************\n%s\n", log.c_str());
	}
}

void CShader::saveProgramInfoLog(GLuint obj)
{
	int len = 0;
	assert(glGetError() == GL_NO_ERROR);
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &len);
	assert(glGetError() == GL_NO_ERROR);

	if (len > 0)
	{
		char* ptr = new char[len + 1];
		GLsizei written = 0;
		glGetProgramInfoLog(obj, len, &written, ptr);
		ptr[written - 1] = '\0';
		assert(glGetError() == GL_NO_ERROR);
		log.append(ptr);
		delete[] ptr;

		printf("LOG **********************************************\n%s\n", log.c_str());
	}
}

GLint CShader::getLocation(const char* varname, loctable* table)
{
	if (varname == 0 || table == 0)
		return 0;

	GLint loc = 0;
	loctable* locs = table;

	loctable::iterator cur = locs->find(varname);

	if (cur == locs->end()) //not found in the locations table
	{
		loc = glGetUniformLocation(program, varname);
		if (loc == -1)
		{
			return -1;
		}

		//insert the new value
		locs->insert(loctable::value_type(varname, loc));
	}
	else //found in the table
	{
		loc = (*cur).second;
	}
	return loc;
}

int CShader::getAttribLocation(const char* varname)
{
	int loc = glGetAttribLocation(program, varname);
	if (loc == -1)
	{
		return loc;
	}
	assert(glGetError() == GL_NO_ERROR);

	return loc;
}

int CShader::getUniformLocation(const char* varname)
{
	int loc = getLocation(varname, &locations);
	if (loc == -1)
	{
		return loc;
	}
	assert(glGetError() == GL_NO_ERROR);
	return loc;
}

void CShader::setTexture(const char* varname, CTexture* tex)
{
	setTexture(varname, tex->m_texture_id);
}

void CShader::setTexture(const char* varname, unsigned int tex)
{
	glActiveTexture(GL_TEXTURE0 + last_slot);
	glBindTexture(GL_TEXTURE_2D, tex);
	setUniform1(varname, last_slot);
	last_slot++;
	glActiveTexture(GL_TEXTURE0 + last_slot);
}

void CShader::setUniform1(const char* varname, int input1)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform1i(loc, input1);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform2(const char* varname, int input1, int input2)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform2i(loc, input1, input2);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform3(const char* varname, int input1, int input2, int input3)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform3i(loc, input1, input2, input3);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform4(const char* varname, const int input1, const int input2, const int input3, const int input4)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform4i(loc, input1, input2, input3, input4);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform1Array(const char* varname, const int* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform1iv(loc, count, input);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform2Array(const char* varname, const int* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform2iv(loc, count, input);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform3Array(const char* varname, const int* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform3iv(loc, count, input);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform4Array(const char* varname, const int* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform4iv(loc, count, input);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform1(const char* varname, const float input1)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform1f(loc, input1);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform2(const char* varname, const float input1, const float input2)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform2f(loc, input1, input2);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform3(const char* varname, const float input1, const float input2, const float input3)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform3f(loc, input1, input2, input3);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform4(const char* varname, const float input1, const float input2, const float input3, const float input4)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform4f(loc, input1, input2, input3, input4);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform1Array(const char* varname, const float* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform1fv(loc, count, input);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform2Array(const char* varname, const float* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform2fv(loc, count, input);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform3Array(const char* varname, const float* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform3fv(loc, count, input);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setUniform4Array(const char* varname, const float* input, const int count)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniform4fv(loc, count, input);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setMatrix44(const char* varname, const float* m)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniformMatrix4fv(loc, 1, GL_FALSE, m);
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::setMatrix44(const char* varname, const glm::mat4 &m)
{
	GLint loc = getLocation(varname, &locations);
	CHECK_SHADER_VAR(loc, varname);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
	assert(glGetError() == GL_NO_ERROR);
}

void CShader::init()
{
	CShader::s_ready = true;
}