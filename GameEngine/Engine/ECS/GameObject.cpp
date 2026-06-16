#include "GameObject.h"

GameObject::GameObject(std::string n, std::string t) : name{n}, tag{t} {}

GameObject::~GameObject()
{
    for (Component* comp : m_components)
    {
        delete comp;
    }
    m_components.clear();
}

void GameObject::Init()
{
    for (const auto& c : m_components)
    {
        c->Init();
    }
}

void GameObject::Update(float dt)
{
    for (const auto& c : m_components)
    {
        c->Update(dt);
    }
}

void GameObject::SetActive(bool act)
{
    isActive = act;
}

bool GameObject::IsAlive() const
{
    return isAlive;
}

void GameObject::Destroy()
{
    isAlive = false;
}