#pragma once

class GameObject;

struct Component
{
    virtual ~Component() = default;

    virtual void Init() {}
    virtual void Update(float deltaTime) {}

	GameObject* GetOwner() const { return m_ownerObject; }

	friend class GameObject; // GameObject can access to m_ownerObject
private:
	GameObject* m_ownerObject = nullptr;
};
