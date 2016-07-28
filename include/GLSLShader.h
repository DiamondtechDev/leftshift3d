#pragma once
#include <string>
#include <GL/glew.h>

class GLSLShader
{
public:
	GLSLShader();
	~GLSLShader();

	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	
	void linkShaders();
	
	void addAttribute(const std::string& attribName);
	
	GLint getUniformLocation(const std::string& uniformName);

	void start();
	void stop();
private:
	int numAttributes_;
	
	GLuint programID_;
	GLuint vertexShaderID_;
	GLuint fragmentShaderID_;
	
	void compileShader(const std::string& filePath, GLuint& id);
};
