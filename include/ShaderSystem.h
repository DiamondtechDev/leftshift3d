#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLSLShader.h"

class ShaderSystem
{
	public:
		ShaderSystem(GLSLShader* shader);
		~ShaderSystem();

		virtual void init();
	protected:
		GLSLShader& shader_;
};
