#pragma once
#include <Box2D/Box2D.h>
#include "ISystem.h"
#include "../ECS/GameObjectManager.h"

//constexpr float PPM = 100; // pixel per meter

class PhysicsSystem : public ISystem
{
public:
	// funtions
	PhysicsSystem(GameObjectManager* = nullptr, float = 0.0f, float = 9.0f);
	~PhysicsSystem() override;

	// Copying and Moving is Forbidden
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem& operator=(const PhysicsSystem&) = delete;
	PhysicsSystem(PhysicsSystem&&) = delete;
	PhysicsSystem& operator=(PhysicsSystem&&) = delete;

	// override
	bool Init() override;
	void Update(float) override;
	void Shutdown() override;
	void Render() override {} // physics do not render

	// Collider types
	b2Body* CreateBox(float, float, float, float, bool, float, float);
	b2Body* CreateCircle(float, float, float, bool, float, float);

	// variables
	const static inline int PPM = 100; // pixel per meter
private:
	float m_gravityX, m_gravityY;
	std::unique_ptr<b2World> m_world = nullptr;
	GameObjectManager* m_gameObjectManager;
};