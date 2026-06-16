#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

using std::vector;

struct Sprite
{
	SDL_Texture* texture;
	SDL_Rect dst;
	SDL_Rect src;
	int zIndex;
	SDL_RendererFlip flip;
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
	void Flush(SDL_Renderer*);

private:
	vector<Sprite> m_sprites;
};