#pragma once
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "InputManager.h"
#include <string>
#include "GLSLShader.h"
#include "FPSLimiter.h"

enum GameState {
	PLAYING,
	EXIT
};

class MainGame
{
	public:
		MainGame();
		~MainGame();

		void run();
	private:
		void initSystems();
		void gameLoop();
		void renderGame();
		void handleEvents();
		void calculateFPS();

		int windowWidth_ = 1280;
		int windowHeight_ = 720;

		SDL_Window *window_;
		const char* title_ = "Game Engine";

		SDL_GLContext glContext_ = nullptr;

		GameState state_;
		InputManager inputManager_;
		FPSLimiter fpsLimiter_;

		float maxFps_ = 60.0f;
		float fps_ = 0;
};
