#pragma once
#include "ISystem.h"
#include "../ECS/GameObjectManager.h"
#include "../Renderer/RenderQueue.h"
#include "../Renderer/Camera2D.h"

class RenderSystem : public ISystem
{
public:
	RenderSystem(GameObjectManager* = nullptr, RenderQueue* = nullptr, Camera2D* = nullptr);
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
	Camera2D* m_camera;
};
