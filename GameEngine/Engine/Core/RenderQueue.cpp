#include "RenderQueue.h"

void RenderQueue::Push(const Sprite& s)
{
	m_sprites.push_back(s);
}

void RenderQueue::Clear()
{
	m_sprites.clear();
}

void RenderQueue::Flush(SDL_Renderer* r)
{
	sort(m_sprites.begin(), m_sprites.end(), [](const Sprite& a, const Sprite& b) {return a.zIndex < b.zIndex; });

	for (const auto& s : m_sprites)
	{
		SDL_RenderCopyEx(r, s.texture, (s.src.w ? &s.src : nullptr), &s.dst, 0.0, nullptr, s.flip);
	}
	m_sprites.clear();
}