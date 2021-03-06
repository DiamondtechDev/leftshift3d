#version 330 core

in vec3 texCoords;

out vec4 color;

uniform samplerCube cubeMapTexture;

void main()
{
    color = texture(cubeMapTexture, texCoords);
}
