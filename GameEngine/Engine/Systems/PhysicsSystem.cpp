#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(GameObjectManager* manager, float gX, float gY) : m_gameObjectManager{ manager }, m_gravityX{ gX }, m_gravityY{ gY } {}
PhysicsSystem::~PhysicsSystem() { Shutdown(); }

bool PhysicsSystem::Init()
{
	m_world = std::make_unique<b2World>(b2Vec2(m_gravityX, m_gravityY));
	return m_world != nullptr;
}

void PhysicsSystem::Shutdown()
{
	m_world.reset();
}

void PhysicsSystem::Update(float dt)
{
	if (!m_world) return;

	m_world->Step(dt, 8, 6);

	if (m_gameObjectManager)
	{
		for (GameObject *obj : m_gameObjectManager->GetObjects())
		{
			if (obj && obj->IsAlive())
			{
				TransformComponent *transform = obj->GetComponent<TransformComponent>();
				RigidbodyComponent* rb = obj->GetComponent<RigidbodyComponent>();

				if (transform && rb && rb->body)
				{
					b2Vec2 pos = rb->body->GetPosition();
					transform->position = Vector2(pos.x, pos.y) * PPM;

					// other operations
				}
			}
		}
	}
}

b2Body* PhysicsSystem::CreateBox(float px, float py, float w, float h, bool dynamic, float density, float rest)
{
	b2BodyDef bd;
	bd.type = dynamic ? b2_dynamicBody : b2_staticBody;
	bd.position.Set(px / PPM, py / PPM);

	b2PolygonShape shape;
	shape.SetAsBox((w / 2) / PPM, (h / 2) / PPM);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = density;
	fd.restitution = rest;

	b2Body* body = m_world->CreateBody(&bd);
	body->CreateFixture(&fd);
	return body;
}

b2Body* PhysicsSystem::CreateCircle(float px, float py, float radius, bool dynamic, float density, float rest)
{
	b2BodyDef bd;
	bd.type = dynamic ? b2_dynamicBody : b2_staticBody;
	bd.position.Set(px / PPM, py / PPM);

	b2CircleShape shape;
	shape.m_radius = radius / PPM;

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = density;
	fd.restitution = rest;

	b2Body* body = m_world->CreateBody(&bd);
	body->CreateFixture(&fd); // collider
	return body;
}