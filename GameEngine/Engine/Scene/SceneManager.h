#pragma once
#include "Scene.h"
#include <vector>
#include <memory>

class SceneManager
{
public:
	SceneManager() = default;
	~SceneManager();

	// Copying and Moving is Forbidden
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	void Push(std::unique_ptr<Scene> scene);
	void Pop();
	void LoadScene(std::unique_ptr<Scene> scene);

	void Update(float);
	void Render();

private:
	int currentSceneIndex = 0;
	std::vector<std::unique_ptr<Scene>> m_scenes;
};