#include "Camera2D.h"
#include "../ECS/Components/TransformComponent.h" 
#include <cstdlib> // rand() 
#include <iostream>

Camera2D::Camera2D(int screenW, int screenH) : m_screenWidth{ screenW }, m_screenHeight{ screenH }, m_target{nullptr}
{
	m_position = { 0.0f, 0.0f };
	m_shakeOffset = { 0.0f, 0.0f };
}

void Camera2D::Update(float dt)
{
	if (m_target && m_target->IsAlive())
	{
		TransformComponent *transform = m_target->GetComponent<TransformComponent>();
		if (transform)
		{
			// Current = Current + (Target - Current) * Speed * dt
			m_position.x += (transform->position.x - m_position.x) * m_followLerp * dt;
			m_position.y += (transform->position.y - m_position.y) * m_followLerp * dt;
		}
	}

	if (m_shakeDuration > 0.0f)
	{
		m_shakeDuration -= dt;
		if (m_shakeDuration > 0.0f)
		{
			// -1.0 - 1.0 
			float randomX = (rand() % 200 - 100) / 100.0f;
			float randomY = (rand() % 200 - 100) / 100.0f;

			m_shakeOffset.x = randomX * m_shakeIntensity;
			m_shakeOffset.y = randomY * m_shakeIntensity;
		}
		else
		{
			// reset
			m_shakeDuration = 0.0f;
			m_shakeOffset = { 0.0f, 0.0f };
		}
	}
}

void Camera2D::Shake(float intensity, float duration)
{
	// we can check if there are any shaking currently occurring.
	m_shakeIntensity = intensity;
	m_shakeDuration = duration;
}

Vector2f Camera2D::WorldToScreen(const Vector2f& worldPos) const
{
	float posX = (worldPos.x - m_position.x) * m_zoom + (m_screenWidth / 2.0f) + m_shakeOffset.x;
	float posY = (worldPos.y - m_position.y) * m_zoom + (m_screenHeight / 2.0f) + m_shakeOffset.y;

	return Vector2f{ posX, posY };
}

Vector2f Camera2D::ScreenToWorld(const Vector2f& screenPos) const
{
	float posX = ((screenPos.x - (m_screenWidth / 2.0f) - m_shakeOffset.x) / m_zoom) + m_position.x;
	float posY = ((screenPos.y - (m_screenHeight / 2.0f) - m_shakeOffset.y) / m_zoom) + m_position.y;

	return Vector2f{ posX, posY };
}

bool Camera2D::IsInView(const SDL_Rect& dstRect) const
{
	// Calculate the borders of the camera (World Space)
	// half of the screen
	float scaledW = m_screenWidth / 2.0f / m_zoom; 
	float scaledH = m_screenHeight / 2.0f / m_zoom; 

	float viewLeft = m_position.x - scaledW;
	float viewRight = m_position.x + scaledW;
	float viewTop = m_position.y - scaledH;
	float viewBottom = m_position.y + scaledH;

	// AABB (Axis-Aligned Bounding Box)
	bool isOutside = (dstRect.x + dstRect.w < viewLeft) ||  
		(dstRect.x > viewRight) ||             
		(dstRect.y + dstRect.h < viewTop) ||   
		(dstRect.y > viewBottom);              

	return !isOutside;
}
