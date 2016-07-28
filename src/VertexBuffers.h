#pragma once

#include <GL/glew.h>
#include <vector>

class VertexBuffers
{
public:
	VertexBuffers();

	void createVBO(int size = 0);
	void releaseVBO();

	void* mapBufferToMemory(int usage);
	void* mapSubBufferToMemory(int usage, GLuint offset, GLuint length);
	void unmapBuffer();

	void bindVBO(int bufferType = GL_ARRAY_BUFFER);
	void uploadDataToGPU(int usage);
	
	void addData(void* data, GLuint dataSize);

	void* getDataPointer();
	GLuint getBuffer();
private:
	GLuint buffer_;
	int size_;
	int bufferType_;
	std::vector<GLbyte> data_;

	bool dataUploaded_ = false;
};
