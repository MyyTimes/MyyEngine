#include "RenderQueue.h"

void RenderQueue::Push(const Sprite& s)
{
	m_sprites.push_back(s);
}

void RenderQueue::Clear()
{
	m_sprites.clear();
}

void RenderQueue::Flush()
{
	sort(m_sprites.begin(), m_sprites.end(), [](const Sprite& a, const Sprite& b) {return a.zIndex < b.zIndex; });

	Renderer2D::BeginScene();

	for (const auto& s : m_sprites)
	{
		Renderer2D::DrawSprite(s.texture, s.srcRect, s.dstRect, s.props);
	}

	Renderer2D::EndScene();
	m_sprites.clear();
}
