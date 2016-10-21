#include "GLSLShader.h"
#include "Errors.h"

#include <fstream>
#include <vector>
#include <iostream>

GLSLShader::GLSLShader() 
{

}

GLSLShader::~GLSLShader() 
{
}

void GLSLShader::compileShader(const std::string& filePath, GLuint& id) 
{
	///* Load shader file
	std::ifstream shaderFile(filePath);
	if(shaderFile.fail())
	{
		perror(filePath.c_str());
		fatalError("Failed to open file @"+filePath);
	}

	std::string fileContents = "";
	std::string line;

	while(std::getline(shaderFile, line))
	{
		fileContents += line + "\n";
	}

	shaderFile.close();

	const char* contentsPointer = fileContents.c_str();

	///* Create the shader with the contents of the file
	glShaderSource(id, 1, &contentsPointer, nullptr);
	glCompileShader(id);

	GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		///* The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(id);

		///* Exit with failure.		
		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader " + filePath + " failed to compile");
	}
}

void GLSLShader::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) 
{
	programID_ = glCreateProgram();
	vertexShaderID_ = glCreateShader(GL_VERTEX_SHADER);
	if(vertexShaderID_ == 0)
	{
		fatalError("Vertex shader failed to be created!");
	}

	fragmentShaderID_ = glCreateShader(GL_FRAGMENT_SHADER);
	if(fragmentShaderID_ == 0)
	{
		fatalError("Fragment shader failed to be created!");
	}
	
	compileShader(vertexShaderFilePath, vertexShaderID_);
	compileShader(fragmentShaderFilePath, fragmentShaderID_);
}

GLuint GLSLShader::getUniformLocation(const std::string& uniformName) 
{
	GLuint location = glGetUniformLocation(programID_, uniformName.c_str());
	if(location == GL_INVALID_INDEX)
	{
		fatalError("Uniform '"+ uniformName +"' not found in shader!");
	}
	return location;
}

void GLSLShader::linkShaders() 
{
	///* Attach our shaders to our program
	glAttachShader(programID_, vertexShaderID_);
	glAttachShader(programID_, fragmentShaderID_);

	///* Link our program
	glLinkProgram(programID_);

	///* Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(programID_, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID_, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programID_, maxLength, &maxLength, &infoLog[0]);
		
		//We don't need the program anymore.
		glDeleteProgram(programID_);
		//Don't leak shaders either.
		glDeleteShader(vertexShaderID_);
		glDeleteShader(fragmentShaderID_);

		std::printf("%s\n", &(infoLog[0]));
		fatalError("Shader linking failed!");
	}

	///* Always detach shaders after a successful link.
	glDetachShader(programID_, vertexShaderID_);
	glDetachShader(programID_, fragmentShaderID_);
	glDeleteShader(vertexShaderID_);
	glDeleteShader(fragmentShaderID_);
}

void GLSLShader::addAttribute(const std::string& attribName) 
{
	glBindAttribLocation(programID_, numAttributes_++, attribName.c_str());
}

void GLSLShader::start() 
{
	glUseProgram(programID_);
	/*for(int i = 0; i < numAttributes_; i++)
	{
		glEnableVertexAttribArray(i);
	}*/
}

void GLSLShader::stop() 
{
	/*for(int i = 0; i < numAttributes_; i++)
	{
		glDisableVertexAttribArray(i);
	}*/
	glUseProgram(0);
}
