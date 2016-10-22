#include "MainGame.h"
#include "Errors.h"
#include <iostream>

MainGame::MainGame() : camera_(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f)
{

}

MainGame::~MainGame()
{
	delete sun;
	delete model;
	delete lighting;
	delete test;
	delete skybox;
	delete skyboxRenderer;
}

void MainGame::run()
{
	initSystems();
	initGame();
	gameLoop();
}

void MainGame::initSystems()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		fatalError(SDL_GetError());

	window_ = SDL_CreateWindow(title_,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		windowWidth_, windowHeight_,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	glContext_ = SDL_GL_CreateContext(window_);
	if (glContext_ == nullptr)
		fatalError("There was an error creating the OpenGL context!");
	
	const unsigned char *version = glGetString(GL_VERSION);
	if (version == nullptr) 
		fatalError("There was an error creating the OpenGL context!");
	std::cout << "*** OpenGL " << version << " ***" << std::endl;
	
	SDL_GL_MakeCurrent(window_, glContext_);
	
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

void MainGame::initGame() {
	myShader.compileShaders("shaders/generic.vert", "shaders/generic.frag");
	myShader.addAttribute("position");
	myShader.addAttribute("texCoord");
	myShader.addAttribute("normals");
	myShader.linkShaders();

	skyboxShader.compileShaders("shaders/skybox.vert", "shaders/skybox.frag");
	skyboxShader.addAttribute("position");
	skyboxShader.linkShaders();

	SDL_ShowCursor(SDL_DISABLE);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	model = new AssimpModel("resources/cube.obj");
	lighting = new Lighting(&myShader);
	lighting->init();

	sun->color = glm::vec3(1.0f, 1.0f, 1.0f);
	sun->ambientIntensity = 0.1f;
	sun->direction = glm::vec3(-1.0f, -1.0f, 0.0f);
	sun->diffuseIntensity = 0.8f;

	test->color = glm::vec3(1.0f, 0.0f, 0.0f);
	test->ambientIntensity = 0.1f;
	test->diffuseIntensity = 0.8f;
	test->attenuation.linear = 2.0f;
	test->position = glm::vec3(0.0f, 0.0f, 2.0f);

	skyboxRenderer = new SkyboxRenderer(&skyboxShader);
	skyboxRenderer->init();

	skybox = new Skybox();
	skybox->init("resources/sor_lake1", "lake1_lf.JPG",
			"lake1_rt.JPG",
			"lake1_up.JPG",
			"lake1_dn.JPG",
			"lake1_ft.JPG",
			"lake1_bk.JPG");
}

void MainGame::gameLoop()
{

	while(state_ != GameState::EXIT)
	{
		handleEvents();

		fpsLimiter_.begin();
		glClearColor(0.0,0.26,0.9,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderGame();

		fps_ = fpsLimiter_.end();
		if(inputManager_.isKeyPressed(SDLK_f))
			std::cout << "Current FPS: " << fps_ << std::endl;
			
		SDL_GL_SwapWindow(window_);
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
			case SDL_MOUSEWHEEL:
				inputManager_.setMouseWheel(e.wheel.y, e.wheel.x);
				break;
		}
	}

	glm::vec2 mousepos = inputManager_.getMouseCoords();	

	if (showMouse_ == false) 
	{
		SDL_SetWindowGrab(window_, SDL_TRUE);
		SDL_WarpMouseInWindow(window_, windowWidth_/2, windowHeight_/2);

		camera_.ProcessMouseMovement(-(windowWidth_/2 - mousepos.x), windowHeight_/2 - mousepos.y, GL_TRUE);
	} else {
		SDL_SetWindowGrab(window_, SDL_FALSE);
	}

	/* Handle inputs */
	//if(inputManager_.isKeyPressed(SDLK_ESCAPE))
	//	state_ = GameState::EXIT;

	if(inputManager_.isKeyDown(SDLK_w))
		camera_.ProcessKeyboard(Camera_Movement::FORWARD, 0.01f);

	if(inputManager_.isKeyDown(SDLK_s))
		camera_.ProcessKeyboard(Camera_Movement::BACKWARD, 0.01f);

	if(inputManager_.isKeyDown(SDLK_d))
		camera_.ProcessKeyboard(Camera_Movement::RIGHT, 0.01f);

	if(inputManager_.isKeyDown(SDLK_a))
		camera_.ProcessKeyboard(Camera_Movement::LEFT, 0.01f);

	camera_.ProcessMouseScroll((float) inputManager_.getMouseWheelVertical() / 10.0f);

	if(inputManager_.isKeyPressed(SDL_BUTTON_LEFT))
		std::cout << "mX: " << mousepos.x << " mY: " << mousepos.y << std::endl;

	if(inputManager_.isKeyPressed(SDLK_ESCAPE))
		{
			showMouse_ = !showMouse_;

			if(showMouse_ == true) {
				SDL_ShowCursor(SDL_ENABLE);
			} else {
				SDL_ShowCursor(SDL_DISABLE);
			}
		}
}

void MainGame::renderGame()
{
	glm::mat4 projection = glm::perspective(camera_.getFOV(), (GLfloat)windowWidth_/(GLfloat)windowHeight_, 0.1f, 100.0f);

	myShader.start();

	GLuint loc_project = myShader.getUniformLocation("mat_project");
	GLuint loc_view = myShader.getUniformLocation("mat_view");
	GLuint loc_model = myShader.getUniformLocation("mat_model");
	GLuint loc_camPos = myShader.getUniformLocation("cameraPosition");
	GLuint loc_tex = myShader.getUniformLocation("ourTexture");

	glm::mat4 mat_model = glm::mat4(1.0f);
	mat_model = glm::translate(mat_model, glm::vec3(0.1f, 0.2f, 0.1f));
	glm::vec3 camPos = camera_.getPosition();

	glUniformMatrix4fv(loc_project, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(camera_.getViewMatrix()));
	glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(mat_model));

	glUniform3f(loc_camPos, camPos.x, camPos.y, camPos.z);

	lighting->setDirectionalLight(*sun);
	lighting->setSpecularIntensity(0.6f);
	lighting->setSpecularPower(1.0f);
	lighting->setPointLights(1, test);
	glUniform1i(loc_tex, 1);

	model->render();

	myShader.stop();

	skyboxShader.start();
	skyboxRenderer->render(&projection, &camera_, skybox);
}
