#version 330 core

in vec3 position;

uniform mat4 mat_project;
uniform mat4 mat_model;
uniform mat4 mat_view;

out vec3 texCoords;

void main()
{
	gl_Position = mat_project * mat_view * mat_model * vec4(position, 1.0);
	texCoords = position;
}
