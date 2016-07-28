#pragma once

#include <string>
#include <GL/glew.h>

#include <SOIL/SOIL.h>

enum TextureFiltering
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

class Texture
{
public:
	Texture();

	bool loadTexture2D(const char* filePath, GLenum format = GL_RGB, bool generateMipmaps = false);
	void bindTexture(GLint textureUnit = 0);
	void releaseTexture();

	void setSamplerParameter(GLenum parameter, GLenum value);
	void setFiltering(GLint magnification, GLint minification);
private:
	int width_, height_;
	GLuint texture_, sampler_;

	const char* path_;

	GLint minification_, magnification_;
};
