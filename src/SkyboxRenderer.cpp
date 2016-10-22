#include "SkyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer(GLSLShader* shader) : ShaderSystem(shader)
{

}

SkyboxRenderer::~SkyboxRenderer()
{

}

void SkyboxRenderer::init() 
{
	cubeMapLocation_ = shader_.getUniformLocation("cubeMapTexture");
	viewMatrixLocation_ = shader_.getUniformLocation("mat_view");
	modelMatrixLocation_ = shader_.getUniformLocation("mat_model");
	projectionMatrixLocation_ = shader_.getUniformLocation("mat_project");
}

void SkyboxRenderer::render(glm::mat4* projection, Camera* camera, Skybox* skybox)
{
	glm::vec3 cameraPos = camera->getPosition();
	glm::mat4 mat_model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z)), 
		glm::vec3(50.0f, 50.0f, 50.0f));

	glUniformMatrix4fv(projectionMatrixLocation_, 1, GL_FALSE, glm::value_ptr(*projection));
	glUniformMatrix4fv(viewMatrixLocation_, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(modelMatrixLocation_, 1, GL_FALSE, glm::value_ptr(mat_model));
	glUniform1i(cubeMapLocation_, 0);
	skybox->render();
}
