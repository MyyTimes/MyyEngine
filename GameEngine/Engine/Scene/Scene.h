#pragma once
#include "../ECS/GameObjectManager.h"
#include "../Systems/InputSystem.h"

class Scene
{
public:
	Scene() : m_input(InputSystem::GetInstance()) {}
	virtual ~Scene() = default;

	// Copying and Moving is Forbidden
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(Scene&&) = delete;

	virtual void OnEnter() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnRender() = 0;
	virtual void OnExit() = 0;

protected:
	// Each scene has its own GameObjects Manager
	GameObjectManager m_gameObjectManager;
	InputSystem& m_input;
};
