#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "../../Core/Props/ParticleProps.h"

struct ParticleComponent : public Component
{
	ParticleProps props;
	bool isEmitting = true;    

	float particlesPerSecond = 20.0f;
	float timeSinceLastEmit = 0.0f; 

	Vector2f offsetPos = { 0.0f, 0.0f };

	TransformComponent* ownersTransform;

	void Init() override;

	void Update(float) override;
};
