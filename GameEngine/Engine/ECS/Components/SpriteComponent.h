#pragma once
#include "Component.h"
#include <SDL2/SDL.h>
#include "../../Renderer/Renderer2D.h"

struct SpriteComponent : public Component
{
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect{ 0, 0, 0, 0 };
	int zIndex = 0;
	SpriteProperties props;
};
