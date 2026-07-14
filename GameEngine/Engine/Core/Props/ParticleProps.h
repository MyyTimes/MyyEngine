#pragma once

#include "../Math/Vector2.h"
#include <SDL2/SDL.h>

// PARTICLE
// Config -> adjustable parameters
struct ParticleProps
{
	Vector2f position;
	Vector2f velocity;
	Vector2f velocityVariation; // spread

	SDL_Color colorBegin, colorEnd;
	float sizeBegin, sizeEnd;
	float sizeVariation;

	float lifeTime;
};
