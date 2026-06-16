#include "GameObjectManager.h"
#include <algorithm>

GameObject* GameObjectManager::CreateGameObject()
{
    // Creating the obj
    auto entity = std::make_unique<GameObject>();

    GameObject* rawPtr = entity.get();

    m_objects.push_back(std::move(entity));
    m_rawObjects.push_back(rawPtr);

    return rawPtr;
}

void GameObjectManager::Update(float dt)
{
    for (GameObject* entity : m_rawObjects)
    {
        if (entity->IsAlive())
        {
            entity->Update(dt);
        }
    }
}

void GameObjectManager::Refresh()
{
    // Erase if isAlive is false
    
    m_rawObjects.erase(
        std::remove_if(m_rawObjects.begin(), m_rawObjects.end(),
            [](GameObject* e) { return !e->IsAlive(); }),
        m_rawObjects.end()
    );

    m_objects.erase(
        std::remove_if(m_objects.begin(), m_objects.end(),
            [](const std::unique_ptr<GameObject>& e) { return !e->IsAlive(); }),
        m_objects.end()
    );
}

void GameObjectManager::Clear()
{
    m_rawObjects.clear();
    m_objects.clear();
}
