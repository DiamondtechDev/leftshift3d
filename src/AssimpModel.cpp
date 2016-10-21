#include "AssimpModel.h"
#include <iostream>

/**
*	AssimpModel constructor, loads the specified filename if supported by Assimp
**/
AssimpModel::AssimpModel(const char *filename)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if(!scene) {
		printf("Unable to load model: %s\n", importer.GetErrorString());
	}

	for(uint i = 0; i < scene->mNumMeshes; ++i) {
		meshes_.push_back(new AssimpMesh(scene->mMeshes[i]));
	}

	if(scene->HasMaterials())
	{
		textures_.resize(scene->mNumMaterials);

		for(uint i = 0; i < scene->mNumMaterials; i++) {
			const aiMaterial* mat = scene->mMaterials[i];
			textures_[i] = NULL;
			if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
				aiString pathname;

				if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &pathname, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					std::string fullPath = std::string("resources/") + pathname.data;
					textures_[i] = new Texture();

					if (!textures_[i]->loadTexture2D(fullPath.c_str(), GL_RGBA, GL_TRUE)) {
						printf("Error loading texture '%s'\n", fullPath.c_str());
						delete textures_[i];
						textures_[i] = NULL;
					}
				}
			}
		}
	}
}

/**
*	Clears all loaded meshes
**/
AssimpModel::~AssimpModel(void)
{
	for(uint i = 0; i < meshes_.size(); i++) {
		delete meshes_.at(i);
	}

	for(uint tex = 0; tex < textures_.size(); ++tex) {
		if(textures_.at(tex) != NULL) {
			delete textures_.at(tex);
		}
	}
	
	meshes_.clear();
}

/**
*	Renders all loaded meshes
**/
void AssimpModel::render() {
	for(uint tex = 0; tex < textures_.size(); ++tex) {
		if(textures_.at(tex) != NULL) {
			textures_.at(tex)->bindTexture(tex);
		}
	}
	for(uint i = 0; i < meshes_.size(); ++i) { 
		meshes_.at(i)->render();
	}
}
