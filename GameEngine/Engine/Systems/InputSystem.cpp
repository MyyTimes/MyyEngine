#include "InputSystem.h"

InputSystem::InputSystem()
{
	m_currKeyboardState = SDL_GetKeyboardState(&m_keyLength);
	m_prevKeyboardState.assign(m_keyLength, 0); // it is vector that holds prev pressed buttons -> initially it is filled by zeros

	m_currMouseState = 0;
	m_prevMouseState = 0;
	m_mousePos = Vector2i{ 0, 0 };
}

void InputSystem::Update()
{
	// Save prev states
	std::copy(m_currKeyboardState, m_currKeyboardState + m_keyLength, m_prevKeyboardState.begin());
	m_prevMouseState = m_currMouseState;

	// Update mouse state
	m_currMouseState = SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// Send new event to all subscribers
		for (const auto& callback : m_subscribers)
		{
			callback(event);
		}
	}
}

bool InputSystem::IsKeyDown(SDL_Scancode key) const
{
	return m_currKeyboardState[key] == 1;
}

bool InputSystem::IsKeyPressed(SDL_Scancode key) const
{
	return m_currKeyboardState[key] == 1 && m_prevKeyboardState[key] == 0;
}

bool InputSystem::IsKeyUp(SDL_Scancode key) const
{
	return m_currKeyboardState[key] == 0 && m_prevKeyboardState[key] == 1;
}

bool InputSystem::IsMouseButtonDown(int button) const
{
	return (m_currMouseState & SDL_BUTTON(button)) != 0;
}

bool InputSystem::IsMouseButtonPressed(int button) const
{
	return (m_currMouseState & SDL_BUTTON(button)) != 0 && (m_prevMouseState & SDL_BUTTON(button)) == 0;
}

bool InputSystem::IsMouseButtonUp(int button) const
{
	return (m_currMouseState & SDL_BUTTON(button)) == 0 && (m_prevMouseState & SDL_BUTTON(button)) != 0;
}

Vector2f InputSystem::GetMousePosition() const
{
	return Vector2f(static_cast<float>(m_mousePos.x), static_cast<float>(m_mousePos.y));
}

// Observer Functions
void InputSystem::AddSubscriber(InputEventCallBack callback)
{
	m_subscribers.push_back(callback);
}

void InputSystem::ClearSubscribers()
{
	m_subscribers.clear();
}
