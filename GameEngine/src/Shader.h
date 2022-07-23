#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource
{
	int status;
	std::string  vertexSource;
	std::string  fragmentSource;
};

class Shader
{
private:
	unsigned int rendererID;
	unsigned int GetUniformLocation(const std::string& name);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filePath);
	std::unordered_map<std::string, int> uniformLocationCache;

public:
	Shader(const std::string& filename);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
	void SetUniform1i(const std::string& name, int i);
	void SetUniformMat4f(const std::string& name, const glm::mat4& mat);
};