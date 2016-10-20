#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

out vec3 outColor;
out vec2 outTexCoord;

uniform mat4 mat_project;
uniform mat4 mat_model;
uniform mat4 mat_view;

void main() {
	gl_Position = mat_project * mat_view * mat_model * vec4(position, 1.0);
	outColor = color;
	outTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
