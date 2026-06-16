#pragma once
#include "ISystem.h"
#include "../ECS/GameObjectManager.h"

class AnimationSystem : public ISystem
{
public:
	AnimationSystem(GameObjectManager* = nullptr);
	~AnimationSystem() override;

	// Copying and Moving is Forbidden
	AnimationSystem(const AnimationSystem&) = delete;
	AnimationSystem& operator=(const AnimationSystem&) = delete;
	AnimationSystem(AnimationSystem&&) = delete;
	AnimationSystem& operator=(AnimationSystem&&) = delete;

	bool Init() override;
	void Shutdown() override;
	void Update(float) override;
	void Render() override {};

private:
	GameObjectManager* m_gameObjectManager;
};