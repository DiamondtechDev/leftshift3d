#pragma once

#include "ShaderSystem.h"

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

struct BaseLight
{
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;

	BaseLight()
	{
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		ambientIntensity = 0.0f;
		diffuseIntensity = 0.0f;
	}

	BaseLight(glm::vec3 c, float ai, float di)
	{
		color = c;
		ambientIntensity = ai;
		diffuseIntensity = di;
	}
};

struct DirectionalLight : public BaseLight
{        
	glm::vec3 direction;

	DirectionalLight()
	{
		direction = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	DirectionalLight(glm::vec3 c, float ai, float di, glm::vec3 dir) : BaseLight(c, ai, di)
	{
		direction = dir;
	}
};

struct PointLight : public BaseLight
{
	glm::vec3 position;

	struct
	{
		float constant;
		float linear;
		float exp;
	} attenuation;

	PointLight()
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		attenuation.constant = 1.0f;
		attenuation.linear = 0.0f;
		attenuation.exp = 0.0f;
	}

	PointLight(glm::vec3 c, float ai, float di, glm::vec3 pos) : BaseLight(c, ai, di)
	{
		position = pos;
		attenuation.constant = 1.0f;
		attenuation.linear = 0.0f;
		attenuation.exp = 0.0f;
	}
};

struct SpotLight : public PointLight
{
	glm::vec3 direction;
	float cutoff;

	SpotLight()
	{
		direction = glm::vec3(0.0f, 0.0f, 0.0f);
		cutoff = 0.0f;
	}

	SpotLight(glm::vec3 c, float ai, float di, glm::vec3 pos, glm::vec3 dir, float cut) : PointLight(c, ai, di, pos)
	{
		direction = dir;
		cutoff = cut;
	}
};

class Lighting : public ShaderSystem
{
	public:
		Lighting(GLSLShader* shader);
		~Lighting();

		void setDirectionalLight(const DirectionalLight& light);
		void setPointLights(unsigned int numLights, const PointLight* pLights);
		void setSpotLights(unsigned int numLights, const SpotLight* pLights);

		void setSpecularIntensity(float intensity);
		void setSpecularPower(float power);

		virtual void init();

	private:
		GLuint colorLocation_;
		GLuint ambientIntensityLocation_;
		GLuint directionLocation_;
		GLuint diffuseIntensityLocation_;

		GLuint specularIntensityLocation_;
		GLuint specularPowerLocation_;
		GLuint numPointLightsLocation_;
		GLuint numSpotLightsLocation_;

		struct {
			GLuint color;
			GLuint ambientIntensity;
			GLuint diffuseIntensity;
			GLuint position;
			struct
			{
				GLuint constant;
				GLuint linear;
				GLuint expn;
			} atten;
		} pointLightsLocation_[MAX_POINT_LIGHTS];

		struct {
			GLuint color;
			GLuint ambientIntensity;
			GLuint diffuseIntensity;
			GLuint position;
			GLuint direction;
			GLuint cutoff;
			struct
			{
				GLuint constant;
				GLuint linear;
				GLuint expn;
			} atten;
		} spotLightsLocation_[MAX_SPOT_LIGHTS];
};
