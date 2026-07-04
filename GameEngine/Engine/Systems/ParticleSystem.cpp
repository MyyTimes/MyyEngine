#include <random>
#include "ParticleSystem.h"

// between -+5%
inline float GenerateRandomCoefficient()
{
	//return ((rand() % 100) / 100.f - 0.5f);
	return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) - 0.5f;
}

ParticleSystem::ParticleSystem(Camera2D* in_camera, SDL_Renderer* in_renderer, int poolSize) : m_camera{in_camera}, m_renderer{in_renderer}, m_poolIndex{0}
{
	m_particlePool.resize(poolSize);
}

bool ParticleSystem::Init()
{
	return m_camera && m_renderer;
}

void ParticleSystem::Emit(const ParticleProps& props)
{
	// Get from pool
	Particle& particle = m_particlePool[m_poolIndex];
	particle.isActive = true;
	particle.position = props.position;

	// Spread -> Random velocity
	particle.velocity.x = props.velocity.x + props.velocityVariation.x * GenerateRandomCoefficient();
	particle.velocity.y = props.velocity.y + props.velocityVariation.y * GenerateRandomCoefficient();

	particle.colorBegin = props.colorBegin;
	particle.colorEnd = props.colorEnd;

	// Random size
	particle.sizeBegin = props.sizeBegin + props.sizeVariation * GenerateRandomCoefficient();
	particle.sizeEnd = props.sizeEnd;

	particle.lifeTime = props.lifeTime;
	particle.lifeRemaining = props.lifeTime;

	m_poolIndex = (m_poolIndex + 1) % m_particlePool.size();
}

void ParticleSystem::Update(float dt)
{
	// Get all props from queue
	for (const auto& props : s_emitQueue)
	{
		Emit(props);
	}
	s_emitQueue.clear();

	for (Particle& particle : m_particlePool)
	{
		if (!particle.isActive) continue;

		if (particle.lifeRemaining <= 0.0f)
		{
			particle.isActive = false;
		}
		else
		{
			particle.lifeRemaining -= dt;
			particle.position += particle.velocity * dt;
		}
	}
}

void ParticleSystem::Render()
{
	if (!m_camera || !m_renderer) return;

	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	for (Particle& particle : m_particlePool)
	{
		if (!particle.isActive) continue;

		float lifeRatio = particle.lifeRemaining / particle.lifeTime;

		// Lerp for size and color
		float currSize = particle.sizeBegin + (particle.sizeEnd - particle.sizeBegin) * lifeRatio;
		Uint8 r = particle.colorBegin.r + (particle.colorEnd.r - particle.colorBegin.r) * lifeRatio;
		Uint8 g = particle.colorBegin.g + (particle.colorEnd.g - particle.colorBegin.g) * lifeRatio;
		Uint8 b = particle.colorBegin.b + (particle.colorEnd.b - particle.colorBegin.b) * lifeRatio;
		Uint8 a = particle.colorBegin.a + (particle.colorEnd.a - particle.colorBegin.a) * lifeRatio;

		Vector2f screenPos = m_camera->WorldToScreen(particle.position);
		float zoom = m_camera->GetZoom();

		SDL_Rect rect;
		rect.w = static_cast<int>(currSize * zoom);
		rect.h = static_cast<int>(currSize * zoom);
		rect.x = static_cast<int>(screenPos.x - (rect.w / 2.0f));
		rect.y = static_cast<int>(screenPos.y - (rect.h / 2.0f));

		SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
		SDL_RenderFillRect(m_renderer, &rect);
	}
}

void ParticleSystem::Shutdown()
{
	m_particlePool.clear();
}

void ParticleSystem::PushParticleProps(const ParticleProps& props)
{
	s_emitQueue.push_back(props);
}
