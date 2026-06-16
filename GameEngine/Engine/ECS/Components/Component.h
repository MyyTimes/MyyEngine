#pragma once

struct Component
{
    virtual ~Component() = default;

    virtual void Init() {}
    virtual void Update(float deltaTime) {}
};