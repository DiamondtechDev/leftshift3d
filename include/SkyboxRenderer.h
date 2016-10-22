#pragma once
#include "Skybox.h"
#include "ShaderSystem.h"
#include "Camera.h"

class SkyboxRenderer : ShaderSystem
{
	public:
		SkyboxRenderer(GLSLShader* shader);
		~SkyboxRenderer();

		virtual void init();
		void render(glm::mat4* projection, Camera* camera, Skybox* skybox);
	private:
		GLuint cubeMapLocation_;
		GLuint viewMatrixLocation_;
		GLuint modelMatrixLocation_;
		GLuint projectionMatrixLocation_;
};
