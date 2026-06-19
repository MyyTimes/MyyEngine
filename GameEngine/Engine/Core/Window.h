#pragma once
#include <string>
#include <SDL2/SDL.h>

class Window
{
public:
	Window() = default;
	~Window() { Shutdown(); }

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

	bool Init(const std::string&, int, int, bool = true);
	void Shutdown();

	void Clear(Uint8 = 0, Uint8 = 0, Uint8 = 0, Uint8 = 255); // rgb-a
	void Present();

	// Window control
	void ToggleFullscreen();
	void SetVSync(bool);

	// Getters
	inline SDL_Renderer* GetRenderer() const { return m_renderer; }
	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
	inline bool IsFullscreen() const { return m_isFullscreen; }

private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	std::string m_title;
	int m_width = 0;
	int m_height = 0;

	bool m_isFullscreen = false;
	bool m_isVSync = true;
};
