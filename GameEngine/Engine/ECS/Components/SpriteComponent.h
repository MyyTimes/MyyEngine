#pragma once
#include "Component.h"
#include <SDL2/SDL.h>

struct SpriteComponent : public Component
{
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect{ 0, 0, 0, 0 };
	int zIndex = 0;

	// SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	// Default: 255, 255, 255, 255 (original color and opaque)
	SDL_Color color{ 255, 255, 255, 255 };

	void Init() override
	{
		srcRect = { 0, 0, 0, 0 };
		zIndex = 1;
		flip = SDL_FLIP_NONE;
		color = { 255, 255, 255, 255 };
	}
};