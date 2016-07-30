#include "MainGame.h"
#include "Errors.h"
#include <iostream>

MainGame::MainGame()
{

}

MainGame::~MainGame()
{

}

void MainGame::run()
{
	initSystems();
	gameLoop();
}

void MainGame::initSystems()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		fatalError("SDL2 missing!\nPlease install SDL2 to run this application.");

	window_ = SDL_CreateWindow(title_,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		windowWidth_, windowHeight_,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	glContext_ = SDL_GL_CreateContext(window_);
	if (glContext_ == nullptr)
		fatalError("There was an error creating the OpenGL context!");
	
	const unsigned char *version = glGetString(GL_VERSION);
	if (version == nullptr) 
		fatalError("There was an error creating the OpenGL context!");
	std::cout << "*** OpenGL " << version << " ***" << std::endl;
	
	SDL_GL_MakeCurrent(window_, glContext_);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	
	//MUST make a context AND make it current BEFORE glewInit()!
	glewExperimental = GL_TRUE;
	GLenum glew_status = glewInit();
	if (glew_status != 0) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		fatalError("GLEW init failed.");
	}

	fpsLimiter_.init(maxFps_);
	state_ = GameState::PLAYING;
}

void MainGame::gameLoop()
{
	float red = 0;
	float green = 0;
	float blue = 0;
	char redi = 1;
	char greeni = 1;
	char bluei = 1;
	while(state_ != GameState::EXIT)
	{
		red = red + 0.0078125 * redi;
		green = green + 0.03125 * greeni;
		blue = blue + 0.015625 * bluei;
		if(red >= 1.0) {redi = -1;}
		if(green >= 1.0) {greeni = -1;}
		if(blue >= 1.0) {bluei = -1;}
		if(red <= 0.0) {redi = 1;}
		if(green <= 0.0) {greeni = 1;}
		if(blue <= 0.0) {bluei = 1;}
		fpsLimiter_.begin();
		handleEvents();
		glClearColor(red,green,blue,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderGame();

		SDL_GL_SwapWindow(window_);

		fps_ = fpsLimiter_.end();
		if(inputManager_.isKeyPressed(SDLK_f))
			std::cout << "Current FPS: " << fps_ << std::endl;
	}
	SDL_GL_DeleteContext(glContext_);
	SDL_Quit();
}

void MainGame::handleEvents()
{
	/* Update the input manager */
	inputManager_.update();

	/* Check for events */
	SDL_Event e;
	while ( SDL_PollEvent(&e) ) 
	{
		switch(e.type)
		{
			case SDL_QUIT:
				state_ = GameState::EXIT;
				break;
			case SDL_KEYDOWN:
				inputManager_.pressKey(e.key.keysym.sym);
				break;
			case SDL_KEYUP:
				inputManager_.releaseKey(e.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				inputManager_.setMouseCoords(e.motion.x, e.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				inputManager_.pressKey(e.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				inputManager_.releaseKey(e.button.button);
				break;
		}
	}

	/* Handle inputs */
	if(inputManager_.isKeyPressed(SDLK_ESCAPE))
		state_ = GameState::EXIT;

	if(inputManager_.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mousepos = inputManager_.getMouseCoords();
		std::cout << "mX: " << mousepos.x << " mY: " << mousepos.y << std::endl;
	}
}

void MainGame::renderGame()
{

}
