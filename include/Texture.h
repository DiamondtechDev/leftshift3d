#pragma once
#include "LSUtil.h"
#include "SOIL2.h"

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
	~Texture();

	bool loadTexture2D(const char* filePath);
	bool loadCubeMapFromTexture(const std::string& directory, const std::string& posXFile, const std::string& negXFile, 
		const std::string& posYFile, const std::string& negYFile, const std::string& posZFile, const std::string& negZFile);
	bool loadCubeMapFromFile(const std::string& filePath);
	void bindTexture(GLint textureUnit = 0);
	void releaseTexture();

	void setSamplerParameter(GLenum parameter, GLenum value);
	void setFiltering(GLint magnification, GLint minification);

	const char* getPath() const { return path_; }
private:
	int width_, height_;
	GLuint texture_, sampler_ = 0;
	GLenum textureType_;

	const char* path_;

	GLint minification_, magnification_;
};
