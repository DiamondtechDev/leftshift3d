#pragma once
#include "LSUtil.h"
#include "InputManager.h"
#include "Texture.h"
#include "GLSLShader.h"
#include "FPSLimiter.h"
#include "Camera.h"
#include "AssimpModel.h"
#include "Lighting.h"
#include "SkyboxRenderer.h"
#include "Skybox.h"
#include "Entity.h"

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
		void initGame();
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

		GLSLShader myShader;
		GLSLShader skyboxShader;

		Camera camera_;

		SkyboxRenderer* skyboxRenderer;
		Skybox* skybox;

		Lighting* lighting;
		DirectionalLight* sun;
		PointLight* pointLight;
		AssimpModel* cube;
		Entity* model;

		bool showMouse_ = false;
};
