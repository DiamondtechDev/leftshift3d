#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>

#include "LSUtil.h"

#include "AssimpMesh.h"
#include "Texture.h"
#include "GLSLShader.h"

class AssimpModel
{
	public:
		AssimpModel(const char* file);
		~AssimpModel(void);

		void render();
	private:
		std::vector<AssimpMesh*> meshes_;
		std::vector<Texture*> textures_;
};
