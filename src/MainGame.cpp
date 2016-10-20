#include "MainGame.h"
#include "Errors.h"
#include <iostream>

MainGame::MainGame() : camera_(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f)
{

}

MainGame::~MainGame()
{

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
	myShader.addAttribute("color");
	myShader.addAttribute("texCoord");
	myShader.linkShaders();

	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};

	glGenVertexArrays(1, &VertexArrayID);
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &elementbuffer);

	glBindVertexArray(VertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	SDL_ShowCursor(SDL_DISABLE);

	glEnable( GL_MULTISAMPLE );

	tex1.loadTexture2D("resources/texture/diamondlogo.png", GL_RGBA, GL_TRUE);
	tex1.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tex1.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tex1.setFiltering(TextureFiltering::TEXTURE_FILTER_MAG_BILINEAR, TextureFiltering::TEXTURE_FILTER_MIN_BILINEAR);
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
	glBindVertexArray(VertexArrayID);
	myShader.start();

	GLuint loc_project = myShader.getUniformLocation("mat_project");
	GLuint loc_view = myShader.getUniformLocation("mat_view");
	GLuint loc_model = myShader.getUniformLocation("mat_model");

	GLuint loc_tex = myShader.getUniformLocation("ourTexture");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(camera_.getFOV(), (GLfloat)windowWidth_/(GLfloat)windowHeight_, 0.1f, 100.0f); 
	glm::mat4 view = camera_.GetViewMatrix();

	tex1.bindTexture(0);
	glUniform1i(loc_tex, 0);

	glUniformMatrix4fv(loc_project, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	myShader.stop();
	glBindVertexArray(0);
}
