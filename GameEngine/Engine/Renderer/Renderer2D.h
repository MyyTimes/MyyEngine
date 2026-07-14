#pragma once
#include <SDL2/SDL.h>
#include <vector>
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
	static inline SDL_Renderer* m_renderer = nullptr; // its owner -> window class

	// BATCH RENDERING
	// Capacity Limits
	static const int MAX_QUADS = 10000;
	static const int MAX_VERTICES = MAX_QUADS * 4;
	static const int MAX_INDICES = MAX_QUADS * 6;
	// Accumulation Sequences (Buffers)
	static inline std::vector<SDL_Vertex> s_vertices;
	static inline std::vector<int> s_indices;
	// Status Trackers
	static inline SDL_Texture* s_currTexture = nullptr;
	static inline int s_quadCount = 0;
	static inline SDL_BlendMode s_currBlendMode = SDL_BLENDMODE_BLEND;
};
