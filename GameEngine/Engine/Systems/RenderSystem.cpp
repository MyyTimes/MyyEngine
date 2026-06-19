#include "RenderSystem.h"
#include <iostream>

RenderSystem::RenderSystem(GameObjectManager* manager, RenderQueue* rq, Camera2D* cam) 
	: m_gameObjectManager{ manager }, m_RQ{ rq }, m_camera{ cam } {}

RenderSystem::~RenderSystem() { Shutdown(); }

bool RenderSystem::Init()
{
	return m_gameObjectManager && m_RQ && m_camera;
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
					float baseW = (sprite->srcRect.w > 0) ? sprite->srcRect.w : 32.0f;
					float baseH = (sprite->srcRect.h > 0) ? sprite->srcRect.h : 32.0f;

					SDL_Rect worldRect;
					worldRect.w = static_cast<int>(baseW * sprite->props.scale.x);
					worldRect.h = static_cast<int>(baseH * sprite->props.scale.y);
					worldRect.x = static_cast<int>(transform->position.x - (worldRect.w / 2.0f));
					worldRect.y = static_cast<int>(transform->position.y - (worldRect.h / 2.0f));

					if (!m_camera->IsInView(worldRect)) continue;

					// Calculate screen position
					Vector2f screenPos = m_camera->WorldToScreen(transform->position);

					SDL_Rect dstRect;
					dstRect.w = static_cast<int>(worldRect.w * m_camera->GetZoom());
					dstRect.h = static_cast<int>(worldRect.h * m_camera->GetZoom());
					dstRect.x = static_cast<int>(screenPos.x - (dstRect.w / 2.0f));  
					dstRect.y = static_cast<int>(screenPos.y - (dstRect.h / 2.0f));

					// Create sprite to push
					Sprite s;
					s.texture = sprite->texture;
					s.srcRect = sprite->srcRect;
					s.dstRect = dstRect;
					s.zIndex = sprite->zIndex;
					s.props = sprite->props;

					m_RQ->Push(s);
				}
			}
		}
	}
	m_RQ->Flush();
}

void RenderSystem::Shutdown()
{
	m_gameObjectManager = nullptr;
	m_RQ = nullptr;
}
