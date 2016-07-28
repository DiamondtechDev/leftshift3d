#include "VertexBuffers.h"

VertexBuffers::VertexBuffers() {

}

void VertexBuffers::createVBO(int size) {
	glGenBuffers(1, &buffer_);
	data_.reserve(size);
	size_ = size;
}

void VertexBuffers::releaseVBO()
{
	glDeleteBuffers(1, &buffer_);
	dataUploaded_ = false;
	data_.clear();
}

void* VertexBuffers::mapBufferToMemory(int usage)
{
	if(!dataUploaded_) 
		return nullptr;
	void* ptr = glMapBuffer(bufferType_, usage);
	return ptr;
}

void* VertexBuffers::mapSubBufferToMemory(int usage, GLuint offset, GLuint length)
{
	if(!dataUploaded_)
		return nullptr;
	void* ptr = glMapBufferRange(bufferType_, offset, length, usage);
	return ptr;
}

void VertexBuffers::unmapBuffer()
{
	glUnmapBuffer(bufferType_);
}

void VertexBuffers::bindVBO(int bufferType)
{
	bufferType_ = bufferType;
	glBindBuffer(bufferType_, buffer_);
}

void VertexBuffers::uploadDataToGPU(int usage)
{
	glBufferData(bufferType_, data_.size(), &data_[0], usage);
	dataUploaded_ = true;
	data_.clear();
}

void VertexBuffers::addData(void* data, GLuint dataSize)
{
	data_.insert(data_.end(), (GLbyte*)data, (GLbyte*)data + dataSize);
}

void* VertexBuffers::getDataPointer()
{
	if(dataUploaded_) return nullptr;
	return (void*)data_[0];
}

GLuint VertexBuffers::getBuffer()
{
	return buffer_;
}
