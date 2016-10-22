#include "Texture.h"
#include "Errors.h"

#include <iostream>

Texture::Texture()
{

}

Texture::~Texture()
{
	if(texture_ != 0) {
		glDeleteTextures(1, &texture_);
	}
}

bool Texture::loadTexture2D(const char* filePath) 
{
	path_ = filePath;
	textureType_ = GL_TEXTURE_2D;

	texture_ = SOIL_load_OGL_texture(
		filePath,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	if(texture_ == 0) {
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
		fatalError("Failed to load texture\n");
	}

	glGenSamplers(1, &sampler_);

	return true;
}

bool Texture::loadCubeMapFromTexture(const std::string& directory,
									 const std::string& posXFile,
									 const std::string& negXFile,
									 const std::string& posYFile,
									 const std::string& negYFile,
									 const std::string& posZFile,
									 const std::string& negZFile) 
{
	path_ = directory.c_str();
	textureType_ = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &texture_);
	glBindTexture(textureType_, texture_);

	unsigned char* imageData = NULL;

	std::string paths[6] = {
		directory+'/'+posXFile,
		directory+'/'+negXFile,
		directory+'/'+posYFile,
		directory+'/'+negYFile,
		directory+'/'+posZFile,
		directory+'/'+negZFile,
	};

	for(unsigned int i = 0; i < 6; i++) {
		imageData = SOIL_load_image(paths[i].c_str(), &width_, &height_, 0, SOIL_LOAD_RGB);

		if(imageData == NULL) {
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
			fatalError("Failed to load texture\n");
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		SOIL_free_image_data(imageData);
	}

	glBindTexture(textureType_, 0);

	return true;
}

bool Texture::loadCubeMapFromFile(const std::string& filePath) 
{
	path_ = filePath.c_str();
	textureType_ = GL_TEXTURE_CUBE_MAP;

	texture_ = SOIL_load_OGL_single_cubemap(
		filePath.c_str(),
		"EWUDNS",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);

	if(texture_ == 0) {
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
		fatalError("Failed to load texture\n");
	}

	glBindTexture(textureType_, texture_);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(textureType_, 0);

	return true;
}

void Texture::bindTexture(GLint textureUnit) 
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(textureType_, texture_);

	if(sampler_ != 0) {
		glBindSampler(textureUnit, sampler_);
	}
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
