#include "RenderSystem.h"
#include <iostream>

RenderSystem::RenderSystem(GameObjectManager* manager, RenderQueue* rq, SDL_Renderer *renderer) 
	: m_gameObjectManager{ manager }, m_RQ{ rq }, m_renderer{renderer} {}

RenderSystem::~RenderSystem() { Shutdown(); }

bool RenderSystem::Init()
{
	return m_gameObjectManager && m_RQ && m_renderer;
}

void RenderSystem::Render()
{
	if (m_gameObjectManager)
	{
		for (GameObject* obj : m_gameObjectManager->GetObjects())
		{
			if (obj && obj->IsAlive())
			{
				TransformComponent *transform = obj->GetComponent<TransformComponent>();
				SpriteComponent *sprite = obj->GetComponent<SpriteComponent>();

				if (transform && sprite && sprite->texture)
				{
					SDL_Rect dstRect;
					dstRect.x = static_cast<int>(transform->position.x);
					dstRect.y = static_cast<int>(transform->position.y);

					dstRect.w = (sprite->srcRect.w > 0) ? sprite->srcRect.w : 32;
					dstRect.h = (sprite->srcRect.h > 0) ? sprite->srcRect.h : 32;

					SDL_SetTextureColorMod(sprite->texture, sprite->color.r, sprite->color.g, sprite->color.b);
					SDL_SetTextureAlphaMod(sprite->texture, sprite->color.a);

					Sprite s;
					s.texture = sprite->texture;
					s.src = sprite->srcRect;
					s.dst = dstRect;
					s.zIndex = sprite->zIndex;
					s.flip = sprite->flip;

					m_RQ->Push(s);
				}
			}
		}
	}
}

void RenderSystem::Shutdown()
{
	m_gameObjectManager = nullptr;
	m_RQ = nullptr;
	m_renderer = nullptr;
}