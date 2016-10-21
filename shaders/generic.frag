#version 330 core

in vec2 outTexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main(){
	color = texture(ourTexture, outTexCoord);
}
