#include "Texture.h"
#include "Errors.h"

#include <iostream>
#include <string>

Texture::Texture() 
{

}

bool Texture::loadTexture2D(const char* filePath, GLenum format, bool generateMipmaps) {
	path_ = filePath;

	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

	unsigned char* image = nullptr;

	if(format == GL_RGBA) {
		image = SOIL_load_image(filePath, &width_, &height_, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, format, GL_UNSIGNED_BYTE, image);
	} else if(format == GL_RGB) {
		image = SOIL_load_image(filePath, &width_, &height_, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, format, GL_UNSIGNED_BYTE, image);
	} else {
		image = SOIL_load_image(filePath, &width_, &height_, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, image);
	}

	if(image == nullptr) {
		printf("No such file %s", filePath);
		fatalError("Failed to load texture\n");
	}

	if(generateMipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	glGenSamplers(1, &sampler_);

	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture::bindTexture(GLint textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glBindSampler(textureUnit, sampler_);
}

void Texture::releaseTexture()
{
	glDeleteTextures(1, &texture_);
}

void Texture::setSamplerParameter(GLenum parameter, GLenum value)
{
	glSamplerParameteri(sampler_, parameter, value);
}

void Texture::setFiltering(GLint magnification, GLint minification)
{
	// Set magnification filter
	if(magnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(sampler_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if(magnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(sampler_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if(minification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if(minification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if(minification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if(minification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if(minification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(sampler_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	minification_ = minification;
	magnification_ = magnification;
}
