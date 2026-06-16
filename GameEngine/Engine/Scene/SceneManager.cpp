#include "SceneManager.h"

SceneManager::~SceneManager()
{
    while (!m_scenes.empty())
    {
        Pop();
    }
}

void SceneManager::Push(std::unique_ptr<Scene> scene)
{
    scene->OnEnter();
    m_scenes.push_back(std::move(scene));
}

void SceneManager::Pop()
{
    if (!m_scenes.empty())
    {
        m_scenes.back()->OnExit();
        m_scenes.pop_back();
    }
}

void SceneManager::LoadScene(std::unique_ptr<Scene> scene)
{
    // Delete old scenes
    while (!m_scenes.empty())
    {
        Pop();
    }
    // Create new scene
    Push(std::move(scene));
}

void SceneManager::Update(float dt)
{
    if (!m_scenes.empty())
    {
        m_scenes.back()->OnUpdate(dt);
    }
}

void SceneManager::Render()
{
    if (!m_scenes.empty())
    {
        m_scenes.back()->OnRender();
    }
}