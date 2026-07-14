#pragma once

#include <string>
#include <vector>
#include <algorithm>

// Components
#include "Components/Component.h"
#include "Components/TransformComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/RigidbodyComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/ParticleComponent.h"

class GameObject
{
public:
    GameObject(std::string = "New_Object", std::string = "Default");
    ~GameObject();

    // Copying is Forbidden 
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    void Init();
    void Update(float);
    void SetActive(bool);
    bool IsAlive() const;
    void Destroy();

    // COMPONENTS
    // > Component functions
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        T* comp = new T(std::forward<Args>(args)...);
		comp->m_ownerObject = this;
        m_components.push_back(comp);
        return comp;
    }

    template<typename T>
    T* GetComponent()
    {
        for (Component* comp : m_components)
        {
            // With `dynamic_cast`, we check if the component in the list is of the type T that we are looking for
            T* casted = dynamic_cast<T*>(comp);
            if (casted != nullptr)
            {
                return casted;
            }
        }
        return nullptr;
    }

    template<typename T>
    bool RemoveComponent()
    {
        for (auto it = m_components.begin(); it != m_components.end(); it++)
        {
            T* casted = dynamic_cast<T*>(*it);
            if (casted != nullptr)
            {
                delete* it;
                m_components.erase(it);
                return true;
            }
        }
        return false;
    }

    // > Component variables
    TransformComponent transform;

    // Variables
    std::string name;
    std::string tag;

private:
    std::vector<Component*> m_components; // all components connected to the obj
    bool isActive = true;
    bool isAlive = true;
};
