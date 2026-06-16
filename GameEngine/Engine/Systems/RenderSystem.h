#pragma once
#include "ISystem.h"
#include "../ECS/GameObjectManager.h"
#include "../Core/RenderQueue.h"
#include <SDL2/SDL.h>

class RenderSystem : public ISystem
{
public:
	RenderSystem(GameObjectManager* = nullptr, RenderQueue* = nullptr, SDL_Renderer* = nullptr);
	~RenderSystem() override;

	// Copying and Moving is Forbidden
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	RenderSystem(RenderSystem&&) = delete;
	RenderSystem& operator=(RenderSystem&&) = delete;

	// override
	bool Init() override;
	void Update(float) override {}
	void Shutdown() override;
	void Render() override;

private:
	GameObjectManager* m_gameObjectManager;
	RenderQueue* m_RQ;
	SDL_Renderer* m_renderer;
};