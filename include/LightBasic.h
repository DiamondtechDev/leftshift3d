#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLSLShader.h"

struct DirectionalLight
{
	glm::vec3 color;
	float ambientIntensity;
	glm::vec3 direction;
	float diffuseIntensity;
};

class LightBasic
{
	public:
		LightBasic(GLSLShader* shader);
		~LightBasic();

		void setDirectionalLight(const DirectionalLight& light);

		void setSpecularIntensity(float intensity);
		void setSpecularPower(float power);
	private:
		GLSLShader& shader_;

		GLuint colorLocation_;
		GLuint ambientIntensityLocation_;
		GLuint directionLocation_;
		GLuint diffuseIntensityLocation_;

		GLuint specularIntensityLocation_;
		GLuint specularPowerLocation_;
};
