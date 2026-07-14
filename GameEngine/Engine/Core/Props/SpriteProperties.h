#pragma once
#include "../Math/Vector2.h"
#include <SDL2/SDL.h>

// Blend Modes
enum class BlendMode
{
	None,       // Opaque
	Blend,      // Alfa
	Additive,   // Glows
	Multiply    // Shadows
};

struct SpriteProperties
{
	float rotation = 0.0f;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	Vector2f scale = { 1.0f, 1.0f };
	SDL_Color colorTint = { 255, 255, 255, 255 };
	BlendMode blendMode = BlendMode::Blend;
};
