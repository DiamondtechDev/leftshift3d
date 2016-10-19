#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 mat_project;
uniform mat4 mat_model;
uniform mat4 mat_view;

void main() {
	gl_Position = mat_project * mat_view * mat_model * vec4(vertexPosition_modelspace, 1.0);
}
