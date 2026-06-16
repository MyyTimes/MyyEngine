#pragma once
#include "GameObject.h"
#include <memory>

class GameObjectManager
{
public:
	GameObjectManager() = default;
	~GameObjectManager() = default;

	// Copying is Forbiden
	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator=(const GameObjectManager&) = delete;

	GameObject* CreateGameObject();

	void Update(float dt);
	void Refresh();
	void Clear();

	const std::vector<GameObject*>& GetObjects() const { return m_rawObjects; }

private:
	std::vector<std::unique_ptr<GameObject>> m_objects;
	std::vector<GameObject*> m_rawObjects;
};