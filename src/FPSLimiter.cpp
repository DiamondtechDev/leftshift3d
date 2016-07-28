#include "FPSLimiter.h"

FPSLimiter::FPSLimiter()
{

}

void FPSLimiter::begin()
{
	startTicks_ = SDL_GetTicks();
}

float FPSLimiter::end()
{
	calculateFPS();

	float frameTicks = SDL_GetTicks() - startTicks_;
	if(1000.0f / maxFps_ > frameTicks)
		SDL_Delay(1000.0f / maxFps_ - frameTicks);

	return fps_;
}

void FPSLimiter::calculateFPS()
{
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();
	
	float currentTicks;
	currentTicks = SDL_GetTicks();

	frameTime_ = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = frameTime_;

	prevTicks = currentTicks;

	int count;
	currentFrame++;

	if(currentFrame < NUM_SAMPLES)
		count = currentFrame;
	else
		count = NUM_SAMPLES;

	float frameTimeAverage = 0;
	for(int i = 0; i < count; i++)
		frameTimeAverage += frameTimes[i];
	frameTimeAverage /= count;

	if(frameTimeAverage > 0)
		fps_ = 1000.0f / frameTimeAverage;
	else
		fps_ = 26.0f;
}