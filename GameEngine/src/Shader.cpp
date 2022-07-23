#include "Shader.h"
#include "GL/glew.h"
#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader(const std::string& filename)
	:rendererID(0)
{
	// Load the shader from a file
	ShaderProgramSource src = ParseShader(filename);

	// Create the shader and get the ID
	rendererID = CreateShader(src.vertexSource, src.fragmentSource);
}

/*
	Delete the shader once out of scope
*/
Shader::~Shader()
{
	glDeleteProgram(rendererID);
}

/*
	Use both the vertex & fragment shader strings to create a shader program on the GPU.
*/
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// Delete both individual shaders since our shader program was created
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error Handling 
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (!result) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // Get the length of the error message
		char* msg = (char*)_malloca(length * sizeof(char)); // Create a new string on the stack
		glGetShaderInfoLog(id, length, &length, msg); // Store the error message in the string

		// Print out the error mesasage
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;
		std::cout << msg << std::endl;

		// Delete the shader
		glDeleteShader(id);

		// Free memory
		_freea(msg);

		return 0;
	}

	return id;

}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	if (stream.fail())
	{
		std::cout << "Could not load shader" << filePath << std::endl;

	}

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::stringstream ss[2]; // One for each shader
	std::string line;
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{

		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos) {
				// Vertex shader
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// Fragment shader
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}

	}

	std::cout << "Loaded shader " << filePath << std::endl;

	return { 1, ss[0].str(), ss[1].str() };
}

void Shader::Bind() const
{
	glUseProgram(rendererID);
}


void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1i(const std::string& name, int i)
{
	glUniform1i(GetUniformLocation(name), i);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{

	if (uniformLocationCache.find(name) != uniformLocationCache.end())
		return uniformLocationCache[name];

	int location = glGetUniformLocation(rendererID, name.c_str());
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}

	uniformLocationCache[name] = location;

	return location;
}