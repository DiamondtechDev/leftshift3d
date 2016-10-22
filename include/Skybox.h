#pragma once
#include <string>
#include "AssimpModel.h"
#include "Texture.h"

class Skybox
{
	public:
		Skybox();
		~Skybox();

		void init(const std::string& directory,
				  const std::string& posXFile,
				  const std::string& negXFile,
				  const std::string& posYFile,
				  const std::string& negYFile,
				  const std::string& posZFile,
				  const std::string& negZFile);

		void render();
	private:
		AssimpModel* model_;
		Texture* cubeMap_;
};
