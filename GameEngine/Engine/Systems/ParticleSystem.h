#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "ISystem.h"
#include "../Core/Math/Vector2.h"
#include "../Renderer/Camera2D.h"

// PARTICLE
// Config -> adjustable parameters
struct ParticleProps
{
	Vector2f position;
	Vector2f velocity;
	Vector2f velocityVariation; // spread

	SDL_Color colorBegin, colorEnd;
	float sizeBegin, sizeEnd;
	float sizeVariation;

	float lifeTime;
};

// A real created particle
struct Particle
{
	Vector2f position;
	Vector2f velocity;
	SDL_Color colorBegin, colorEnd;
	float sizeBegin, sizeEnd;

	float lifeTime;
	float lifeRemaining;
	bool isActive;
};


// PARTICLE SYSTEM
class ParticleSystem : public ISystem
{
public:
	ParticleSystem(Camera2D* = nullptr, SDL_Renderer* = nullptr, int = 1000);
	~ParticleSystem() = default;


	bool Init() override;
	void Update(float) override;
	void Shutdown() override;
	void Render() override;

	static void PushParticleProps(const ParticleProps&);
private:
	// Spread/Emit new particle
	void Emit(const ParticleProps&);
	// Waiting list for particle props
	inline static std::vector<ParticleProps> s_emitQueue;

	std::vector<Particle> m_particlePool;
	int m_poolIndex = 0;
	Camera2D* m_camera = nullptr;
	SDL_Renderer* m_renderer = nullptr;
};



