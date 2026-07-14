#pragma once
#include <SDL2/SDL.h>
#include "../Core/Props/SpriteProperties.h"
#include "../Core/Math/Vector2.h" 

class Renderer2D
{
public:
	Renderer2D() = default;
	~Renderer2D() = default;

	static void Init(SDL_Renderer* renderer);
	static void Shutdown();

	static void BeginScene();
	static void EndScene();
	static void Flush(); 

	// Texture functions
	// with pos
	static void DrawSprite(SDL_Texture*, const Vector2f&, const SpriteProperties & = SpriteProperties());
	// with sprite sheet -> srcRect, dstRect
	static void DrawSprite(SDL_Texture*, const SDL_Rect&, const SDL_Rect&, const SpriteProperties & = SpriteProperties());

	static void DrawRect(const Vector2f&, const Vector2f&, const SDL_Color&, bool = false);
	static void DrawCircle(const Vector2f&, float, const SDL_Color&, bool = false);
	static void DrawLine(const Vector2f&, const Vector2f&, const SDL_Color&);
	// Drawing text
	static void DrawText(SDL_Texture*, const Vector2f&, const SpriteProperties & = SpriteProperties());

private:
	static SDL_Renderer* m_renderer; // its owner -> window class

	// İleride Batch Rendering verilerini (Vertex Buffer vb.) burada saklayacağız.
	// Şimdilik API arayüzünü temiz tutuyoruz.
};
