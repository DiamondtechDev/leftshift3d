#version 330 core

in vec2 outTexCoord;
in vec3 outNormal;
in vec3 toCameraVector;

out vec4 color;

struct DirectionalLight {
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	vec3 direction;
};

uniform DirectionalLight aDirectionalLight;
uniform sampler2D ourTexture;

uniform float specularIntensity;
uniform float specularPower;

void main() {
	vec4 ambientColor = vec4(aDirectionalLight.color * aDirectionalLight.ambientIntensity, 1.0);
	vec3 lightDirection = -aDirectionalLight.direction;
	vec3 normal = normalize(outNormal);
	float diffuseFactor = dot(normal, lightDirection);
	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0) {
		diffuseColor = vec4(aDirectionalLight.color, 1.0) *
            aDirectionalLight.diffuseIntensity *
            diffuseFactor;

        vec3 lightReflect = normalize(reflect(aDirectionalLight.direction, normal));
        float specularFactor = dot(toCameraVector, lightReflect);
        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, specularPower);
            specularColor = vec4(aDirectionalLight.color * specularIntensity * specularFactor, 1.0f);
        }
	}

	color = texture(ourTexture, outTexCoord) * (ambientColor + diffuseColor + specularColor);
}
