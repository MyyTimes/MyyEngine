#pragma once
#include "../Core/Math/Vector2.h"
#include "../ECS/GameObject.h"
#include <SDL2/SDL.h>

class Camera2D
{
public:
	Camera2D(int = 800, int = 800);
	~Camera2D() = default;

	// Copying and Moving is Forbidden
	Camera2D(const Camera2D&) = delete;
	Camera2D& operator=(const Camera2D&) = delete;
	Camera2D(Camera2D&&) = delete;
	Camera2D& operator=(Camera2D&&) = delete;

	void Update(float);

	// Setters & Getters
	void SetPosition(const Vector2f& pos) { m_position = pos; }
	Vector2f GetPosition() const { return m_position; }

	void SetZoom(float zoom) { m_zoom = zoom; }
	float GetZoom() const { return m_zoom; }

	// Following target
	void SetTarget(GameObject* target) { m_target = target; }

	// Shaking camera
	void Shake(float, float);

	// Scaling
	Vector2f WorldToScreen(const Vector2f&) const;
	Vector2f ScreenToWorld(const Vector2f&) const;

	// FRUSTUM CULLING
	bool IsInView(const SDL_Rect&) const;

private:
	Vector2f m_position; // mid point of camera
	float m_zoom = 1.0f;
	//float m_rotation = 0.0f;

	int m_screenWidth;
	int m_screenHeight;

	// Following
	GameObject* m_target = nullptr;
	float m_followLerp = 5.0f; 

	// Shaking
	float m_shakeDuration = 0.0f;
	float m_shakeIntensity = 0.0f;
	Vector2f m_shakeOffset;
};
