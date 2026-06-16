#pragma once
#include <Box2D/Box2D.h>
#include "Component.h"

struct RigidbodyComponent : public Component
{
    b2Body* body = nullptr;
    bool isStatic = false;
    float density = 0.0f;
    float friction = 0.0f;

    void ApplyForce(float fx, float fy) 
    { 
        if (body) body->ApplyForceToCenter(b2Vec2(fx, fy), true); 
    }

    void SetVelocity(float vx, float vy) 
    { 
        if (body) body->SetLinearVelocity(b2Vec2(vx, vy)); 
    }
};