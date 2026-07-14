#include "ParticleComponent.h"
#include "../../Systems/ParticleSystem.h"
#include "../GameObject.h" 

void ParticleComponent::Init()
{
	timeSinceLastEmit = 0.0f;
	ownersTransform = GetOwner()->GetComponent<TransformComponent>();
}

void ParticleComponent::Update(float deltaTime)
{
	if (!isEmitting || !ownersTransform) return;

	timeSinceLastEmit += deltaTime;
	float emitInterval = 1.0f / particlesPerSecond;

	// If we miss emitting, we will emit consecutively in a loop. 
	while (timeSinceLastEmit >= emitInterval)
	{
		ParticleProps currProps = props;
		currProps.position.x = ownersTransform->position.x + offsetPos.x;
		currProps.position.y = ownersTransform->position.y + offsetPos.y;

		// Push to queue to emit
		ParticleSystem::PushParticleProps(currProps);

		timeSinceLastEmit -= emitInterval;
	}
}
