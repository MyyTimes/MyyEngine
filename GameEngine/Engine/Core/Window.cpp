#include "Window.h"
#include <iostream>

bool Window::Init(const std::string& title, int w, int h, bool isVSync)
{
	m_title = title;
	m_width = w;
	m_height = h;
	m_isVSync = isVSync;

	m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (!m_window)
	{
		std::cerr << "Window Initialization Failed: " << SDL_GetError() << std::endl;
		return false;
	}

	Uint32 renderFlags = SDL_RENDERER_ACCELERATED;
	if (m_isVSync)
	{
		renderFlags |= SDL_RENDERER_PRESENTVSYNC;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, renderFlags);
	if (!m_renderer)
	{
		std::cerr << "Renderer Initialization Failed: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

void Window::Shutdown()
{
	if (m_renderer)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
}

void Window::Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	SDL_RenderClear(m_renderer);
}

void Window::Present()
{
	SDL_RenderPresent(m_renderer);
}

void Window::ToggleFullscreen()
{
	m_isFullscreen = !m_isFullscreen;

	if (m_isFullscreen)
	{
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		SDL_SetWindowFullscreen(m_window, 0);
	}
}

void Window::SetVSync(bool enable)
{
	m_isVSync = enable;
	SDL_RenderSetVSync(m_renderer, enable ? 1 : 0);
}

