#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <vector>

#include "Texture.h"
#include "GLSLShader.h"

enum BUFFERS {
	VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
};

class AssimpMesh
{
	public:
		AssimpMesh(aiMesh *mesh);
		~AssimpMesh();
		
		GLuint vao;
		GLuint vbo[4];

		uint elementCount;

		void load(aiMesh *mesh);
		void render();
};
