#pragma once
#include "../../Core/Math/Vector2.h"
#include "Component.h"

struct TransformComponent : public Component
{
    Vector2<> position;
    Vector2<> size;
    float rotation = 0.0f;
    //Vector2 angular;
};
