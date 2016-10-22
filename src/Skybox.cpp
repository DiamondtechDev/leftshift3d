#include "Skybox.h"
#include <iostream>

Skybox::Skybox()
{

}

Skybox::~Skybox()
{
	delete model_;
	delete cubeMap_;
}

void Skybox::init(const std::string& directory,
				  const std::string& posXFile,
				  const std::string& negXFile,
				  const std::string& posYFile,
				  const std::string& negYFile,
				  const std::string& posZFile,
				  const std::string& negZFile)
{
	model_ = new AssimpModel("resources/sphere.obj");

	cubeMap_ = new Texture();
	cubeMap_->loadCubeMapFromTexture(directory, posXFile, negXFile, posYFile, negYFile, posZFile, negZFile);
}

void Skybox::render()
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	cubeMap_->bindTexture(0);
	model_->render();
	glCullFace(OldCullFaceMode); 
    glDepthFunc(OldDepthFuncMode);
}
