#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include "Renderer2D.h"

using std::vector;

struct Sprite
{
	SDL_Texture* texture;
	SDL_Rect dstRect;
	SDL_Rect srcRect;
	int zIndex;
	SpriteProperties props;
};

class RenderQueue
{
public:
	RenderQueue() = default;
	~RenderQueue() = default;

	// Copying and Moving is Forbidden
	RenderQueue(const RenderQueue&) = delete;
	RenderQueue& operator=(const RenderQueue&) = delete;
	RenderQueue(RenderQueue&&) = delete;
	RenderQueue& operator=(RenderQueue&&) = delete;

	void Push(const Sprite&);
	void Clear();
	void Flush();

private:
	vector<Sprite> m_sprites;
};
