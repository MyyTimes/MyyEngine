#include "AnimationSystem.h"
#include <iostream>
AnimationSystem::AnimationSystem(GameObjectManager* manager) : m_gameObjectManager{manager} {}
AnimationSystem::~AnimationSystem() { Shutdown(); };

bool AnimationSystem::Init()
{
	return m_gameObjectManager;
}

void AnimationSystem::Shutdown()
{
	m_gameObjectManager = nullptr;
}

void AnimationSystem::Update(float dt)
{
	if (m_gameObjectManager)
	{
		for (GameObject* obj : m_gameObjectManager->GetObjects())
		{
			if (obj && obj->IsAlive())
			{
				AnimationComponent* anim = obj->GetComponent<AnimationComponent>();

				if (!anim || !anim->isPlaying || !anim->currentClip || anim->currentClip->frames.empty() || anim->currentClip->fps <= 0.f)
					continue;

				anim->time += dt;

				const float frameDuration = 1.0f / anim->currentClip->fps;
				const int totalFrames = static_cast<int>(anim->currentClip->frames.size());

				while (anim->time >= frameDuration && anim->isPlaying)
				{
					anim->time -= frameDuration;

					if (anim->frameIndex + 1 < totalFrames)
					{
						anim->frameIndex++;
					}
					else // reached last frame
					{
						if (anim->currentClip->loop)
						{
							anim->frameIndex = 0;
						}
						else
						{
							anim->isPlaying = false;
						}
					}
				}
				
				SpriteComponent* sprite = obj->GetComponent<SpriteComponent>();

				if (sprite)
				{
					sprite->texture = anim->currentClip->texture;
					sprite->srcRect = anim->currentClip->frames[anim->frameIndex];
				}
			}
		}
	}
}
