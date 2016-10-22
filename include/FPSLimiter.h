#pragma once
#include "LSUtil.h"

class FPSLimiter
{
public:
	FPSLimiter();
	void init(float targetFPS) { maxFps_ = targetFPS; }

	void setMaxFPS(float targetFPS) { maxFps_ = targetFPS; }

	void begin();
	float end();
private:
	void calculateFPS();

	unsigned int startTicks_;

	float maxFps_;
	float fps_;
	float frameTime_;
};
