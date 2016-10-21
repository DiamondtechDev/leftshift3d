#version 330 core

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

in vec2 outTexCoord;
in vec3 outNormal;
in vec3 toCameraVector;
in vec3 positionVector;

out vec4 color;

struct BaseLight 
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

struct Attenuation
{
	float constant;
	float linear;
	float expn;
};

struct PointLight
{
	BaseLight base;
	vec3 position;
	Attenuation atten;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float cutoff;
};

uniform DirectionalLight aDirectionalLight;
uniform sampler2D ourTexture;

uniform float specularIntensity;
uniform float specularPower;
uniform int numPointLights;
uniform int numSpotLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

vec4 CalcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal)
{
	vec4 ambientColor = vec4(light.color * light.ambientIntensity, 1.0);
	float diffuseFactor = dot(normal, -lightDirection);

	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0) {
		diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0);

		vec3 lightReflect = normalize(reflect(lightDirection, normal));
		float specularFactor = dot(toCameraVector, lightReflect);
		if (specularFactor > 0) {
			specularFactor = pow(specularFactor, specularPower);
			specularColor = vec4(light.color * specularIntensity * specularFactor, 1.0);
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight(vec3 normal)
{
	return CalcLightInternal(aDirectionalLight.base, aDirectionalLight.direction, normal);
}

vec4 CalcPointLight(PointLight l, vec3 normal)
{
	vec3 lightDirection = positionVector - l.position;
	float dist = length(lightDirection);
	lightDirection = normalize(lightDirection);

	vec4 color = CalcLightInternal(l.base, lightDirection, normal);
	float attenuation = l.atten.constant +
		l.atten.linear * dist +
		l.atten.expn * dist * dist;

	return color / attenuation;
}

vec4 CalcSpotLight(SpotLight l, vec3 normal)
{
	vec3 lightToPixel = normalize(positionVector - l.base.position);
	float spotFactor = dot(lightToPixel, l.direction);

	if (spotFactor > l.cutoff) {
		vec4 color = CalcPointLight(l.base, normal);
		return color * (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - l.cutoff));
	} else {
		return vec4(0,0,0,0);
	}
}

void main() {
	vec3 normal = normalize(outNormal);
	vec4 totalLight = CalcDirectionalLight(normal);

	for (int i = 0 ; i < numPointLights ; i++) {
		totalLight += CalcPointLight(pointLights[i], normal);
	}

	for (int i = 0 ; i < numSpotLights ; i++) {
		totalLight += CalcSpotLight(spotLights[i], normal);
	}

	color = texture2D(ourTexture, outTexCoord.xy) * totalLight;
}
