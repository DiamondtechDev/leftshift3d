#include "Lighting.h"
#include <iostream>

Lighting::Lighting(GLSLShader* shader) : ShaderSystem(shader)
{

}

Lighting::~Lighting()
{

}

void Lighting::init()
{
	shader_.start();

	/* The "sun" */
	colorLocation_ = shader_.getUniformLocation("aDirectionalLight.base.color");
	ambientIntensityLocation_ = shader_.getUniformLocation("aDirectionalLight.base.ambientIntensity");
	diffuseIntensityLocation_ = shader_.getUniformLocation("aDirectionalLight.base.diffuseIntensity");
	directionLocation_ = shader_.getUniformLocation("aDirectionalLight.direction");

	/* Specular */
	specularIntensityLocation_ = shader_.getUniformLocation("specularIntensity");
	specularPowerLocation_ = shader_.getUniformLocation("specularPower");

	/* Point lights */
	numPointLightsLocation_ = shader_.getUniformLocation("numPointLights");

	for(uint i = 0; i < MAX_POINT_LIGHTS; i++) {
		pointLightsLocation_[i].color = shader_.getUniformLocation(std::string("pointLights[")+
			std::to_string(i)+"].base.color");
		pointLightsLocation_[i].ambientIntensity = shader_.getUniformLocation(std::string("pointLights[")+
			std::to_string(i)+"].base.ambientIntensity");
		pointLightsLocation_[i].diffuseIntensity = shader_.getUniformLocation(std::string("pointLights[")+
			std::to_string(i)+"].base.diffuseIntensity");
		pointLightsLocation_[i].position = shader_.getUniformLocation(std::string("pointLights[")+
			std::to_string(i)+"].position");
		pointLightsLocation_[i].atten.linear = shader_.getUniformLocation(std::string("pointLights[")+
			std::to_string(i)+"].atten.linear");
		pointLightsLocation_[i].atten.constant = shader_.getUniformLocation(std::string("pointLights[")+
			std::to_string(i)+"].atten.constant");
		pointLightsLocation_[i].atten.expn = shader_.getUniformLocation(std::string("pointLights[")+
			std::to_string(i)+"].atten.expn");
	}

	/* Spot lights */
	numSpotLightsLocation_ = shader_.getUniformLocation("numSpotLights");

	for(uint i = 0; i < MAX_SPOT_LIGHTS; i++) {
		spotLightsLocation_[i].color = shader_.getUniformLocation(std::string("spotLights[")+
			std::to_string(i)+"].base.base.color");
		spotLightsLocation_[i].direction = shader_.getUniformLocation(std::string("spotLights[")+
			std::to_string(i)+"].direction");
		spotLightsLocation_[i].cutoff = shader_.getUniformLocation(std::string("spotLights[")+
			std::to_string(i)+"].cutoff");
		spotLightsLocation_[i].ambientIntensity = shader_.getUniformLocation(std::string("spotLights[")+
			std::to_string(i)+"].base.base.ambientIntensity");
		spotLightsLocation_[i].diffuseIntensity = shader_.getUniformLocation(std::string("spotLights[")+
			std::to_string(i)+"].base.base.diffuseIntensity");
		spotLightsLocation_[i].position = shader_.getUniformLocation(std::string("spotLights[")+
			std::to_string(i)+"].base.position");
		spotLightsLocation_[i].atten.linear = shader_.getUniformLocation(std::string("spotLights[")+
			std::to_string(i)+"].base.atten.linear");
		spotLightsLocation_[i].atten.constant = shader_.getUniformLocation(std::string("spotLights[")+
			std::to_string(i)+"].base.atten.constant");
		spotLightsLocation_[i].atten.expn = shader_.getUniformLocation(std::string("spotLights[")+
			std::to_string(i)+"].base.atten.expn");
	}

	shader_.stop();
}

void Lighting::setDirectionalLight(const DirectionalLight& light) 
{
	glUniform3f(colorLocation_, light.color.x, light.color.y, light.color.z);
	glUniform1f(ambientIntensityLocation_, light.ambientIntensity);
	
	glm::vec3 direction = glm::normalize(light.direction);

	glUniform3f(directionLocation_, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation_, light.diffuseIntensity);
}

void Lighting::setPointLights(unsigned int numLights, const PointLight* pLights)
{
	glUniform1i(numPointLightsLocation_, numLights);

	for (uint i = 0 ; i < numLights ; i++) {
		glUniform3f(pointLightsLocation_[i].color, pLights[i].color.x, pLights[i].color.y, pLights[i].color.z);
		glUniform1f(pointLightsLocation_[i].ambientIntensity, pLights[i].ambientIntensity);
		glUniform1f(pointLightsLocation_[i].diffuseIntensity, pLights[i].diffuseIntensity);
		glUniform3f(pointLightsLocation_[i].position, pLights[i].position.x, pLights[i].position.y, pLights[i].position.z);
		glUniform1f(pointLightsLocation_[i].atten.constant, pLights[i].attenuation.constant);
		glUniform1f(pointLightsLocation_[i].atten.linear, pLights[i].attenuation.linear);
		glUniform1f(pointLightsLocation_[i].atten.expn, pLights[i].attenuation.exp);
	}
}

void Lighting::setSpotLights(unsigned int numLights, const SpotLight* pLights)
{
	glUniform1i(numSpotLightsLocation_, numLights);

	for (uint i = 0 ; i < numLights ; i++) {
		glUniform3f(spotLightsLocation_[i].color, pLights[i].color.x, pLights[i].color.y, pLights[i].color.z);
		glUniform3f(spotLightsLocation_[i].direction, pLights[i].direction.x, pLights[i].direction.y, pLights[i].direction.z);
		glUniform1f(spotLightsLocation_[i].ambientIntensity, pLights[i].ambientIntensity);
		glUniform1f(spotLightsLocation_[i].cutoff, pLights[i].cutoff);
		glUniform1f(spotLightsLocation_[i].diffuseIntensity, pLights[i].diffuseIntensity);
		glUniform3f(spotLightsLocation_[i].position, pLights[i].position.x, pLights[i].position.y, pLights[i].position.z);
		glUniform1f(spotLightsLocation_[i].atten.constant, pLights[i].attenuation.constant);
		glUniform1f(spotLightsLocation_[i].atten.linear, pLights[i].attenuation.linear);
		glUniform1f(spotLightsLocation_[i].atten.expn, pLights[i].attenuation.exp);
	}
}

void Lighting::setSpecularIntensity(float intensity)
{
	glUniform1f(specularIntensityLocation_, intensity);
}

void Lighting::setSpecularPower(float power){
	glUniform1f(specularPowerLocation_, power);
}
