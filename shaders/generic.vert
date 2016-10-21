#version 330 core

in vec3 position;
in vec2 texCoord;
in vec3 normals;

out vec2 outTexCoord;
out vec3 outNormal;
out vec3 toCameraVector;
out vec3 positionVector;

uniform vec3 cameraPosition;

uniform mat4 mat_project;
uniform mat4 mat_model;
uniform mat4 mat_view;

void main() {
	gl_Position = mat_project * mat_view * mat_model * vec4(position, 1.0);
	
	outTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
	outNormal = (mat_model * vec4(normals, 0.0)).xyz;
	positionVector = (mat_model * vec4(position, 1.0)).xyz;
	toCameraVector = cameraPosition - position;
}
