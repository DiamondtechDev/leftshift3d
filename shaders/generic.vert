#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normals;

out vec2 outTexCoord;

uniform mat4 mat_project;
uniform mat4 mat_model;
uniform mat4 mat_view;

void main() {
	gl_Position = mat_project * mat_view * mat_model * vec4(position, 1.0);
	outTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
