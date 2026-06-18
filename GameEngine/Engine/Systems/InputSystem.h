#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <functional>
#include "../Core/Math/Vector2.h"

class InputSystem
{
public:
	static InputSystem& GetInstance()
	{
		static InputSystem instance;
		return instance;
	}

	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;

	void Update();

	// Keyboard
	bool IsKeyDown(SDL_Scancode) const;     
	bool IsKeyPressed(SDL_Scancode) const;  
	bool IsKeyUp(SDL_Scancode) const; 

	// Mouse
	bool IsMouseButtonDown(int) const;
	bool IsMouseButtonPressed(int) const;
	bool IsMouseButtonUp(int) const;
	Vector2f GetMousePosition() const;

	// OBSERVER PATTERN 
	using InputEventCallBack = std::function<void(const SDL_Event&)>;
	void AddSubscriber(InputEventCallBack);
	void ClearSubscribers();

private:
	InputSystem();
	~InputSystem() = default;

	// Keyboard
	const Uint8* m_currKeyboardState;
	std::vector<Uint8> m_prevKeyboardState;
	int m_keyLength;

	// Mouse
	Uint32 m_currMouseState;
	Uint32 m_prevMouseState;
	Vector2i m_mousePos;

	// Observers - subscribers
	std::vector<InputEventCallBack> m_subscribers;
};
