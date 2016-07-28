#include "Errors.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>

void fatalError(std::string error)
{
	std::cout << error << std::endl;
	std::cout << "Application terminated due to errors." << std::endl;
	SDL_Quit();
	exit(1);
}