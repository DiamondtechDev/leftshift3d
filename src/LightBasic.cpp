#include "LightBasic.h"

LightBasic::LightBasic(GLSLShader* shader) : shader_(*shader)
{
	shader_.start();
	colorLocation_ = shader_.getUniformLocation("aDirectionalLight.color");
	ambientIntensityLocation_ = shader_.getUniformLocation("aDirectionalLight.ambientIntensity");
	directionLocation_ = shader_.getUniformLocation("aDirectionalLight.direction");
	diffuseIntensityLocation_ = shader_.getUniformLocation("aDirectionalLight.diffuseIntensity");

	specularIntensityLocation_ = shader_.getUniformLocation("specularIntensity");
	specularPowerLocation_ = shader_.getUniformLocation("specularPower");
	shader_.stop();
}

LightBasic::~LightBasic()
{

}

void LightBasic::setDirectionalLight(const DirectionalLight& light) 
{
	glUniform3f(colorLocation_, light.color.x, light.color.y, light.color.z);
	glUniform1f(ambientIntensityLocation_, light.ambientIntensity);
	
	glm::vec3 direction = glm::normalize(light.direction);

    glUniform3f(directionLocation_, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation_, light.diffuseIntensity);
}

void LightBasic::setSpecularIntensity(float intensity)
{
	glUniform1f(specularIntensityLocation_, intensity);
}

void LightBasic::setSpecularPower(float power){
	glUniform1f(specularPowerLocation_, power);
}
