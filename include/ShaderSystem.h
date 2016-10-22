#pragma once
#include "LSUtil.h"
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
