#include <iostream>
#include <SDL2/SDL.h>
#include "Game.h"

constexpr float FIXED_DT = 1.0f / 60.0f;  // Fixed timestep for physics
constexpr float MAX_DT = 0.05f;            // Cap delta time to prevent spiral of death

Game::Game() {}
Game::~Game() { Shutdown(); }

bool Game::Init(const std::string& title, int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	// SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		std::cerr << "SDL_Init Failed: " << SDL_GetError() << std::endl;
		return false;
	}

	// Window
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_SHOWN);
	if (!m_window)
	{
		std::cerr << "Window Failed: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	// Renderer
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_renderer)
	{
		std::cerr << "Renderer Failed: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
		SDL_Quit();
		return false;
	}

	// Textures and Asset Manager
	m_assets.Init(m_renderer);

	// Fonts
	m_fonts.Init(m_renderer);
	SDL_Color white{ 255,255,255,255 };
	m_fontTexture = m_fonts.GetTextTexture("FPS", "Assets/Fonts/den.ttf", 18, white);
	if (m_fontTexture)
		SDL_QueryTexture(m_fontTexture, nullptr, nullptr, &m_fontDst.w, &m_fontDst.h);

	// Sounds
	m_sounds.Init();
	m_sounds.GetSound("background", "Assets/Sounds/bg_sound.mp3");
	m_sounds.Play("background");

	// Load DEMOSCENE
	m_sceneManager.LoadScene(std::make_unique<DemoGame>(m_renderer, &m_RQ, &m_assets));

	m_isRunning = true;

	return true;
}

void Game::Run()
{
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 freq = SDL_GetPerformanceFrequency();
	Uint32 fpsTimer = SDL_GetTicks();
	int frameCount = 0;
	float accumulator = 0.0f;

	while (m_isRunning)
	{
		Uint64 curr = SDL_GetPerformanceCounter();
		float dt = static_cast<float>(curr - now) / freq;
		now = curr;

		// Cap delta time to prevent spiral of death after breakpoints/pauses
		if (dt > MAX_DT) dt = MAX_DT;

		frameCount++;

		ProcessInput();

		// Fixed timestep accumulator for physics stability
		accumulator += dt;
		while (accumulator >= FIXED_DT)
		{
			Update(FIXED_DT);
			accumulator -= FIXED_DT;
		}

		Render();
		// VSync handles frame pacing — no SDL_Delay needed

		// FPS counter
		if (SDL_GetTicks() - fpsTimer >= 1000)
		{
			std::string txt = "FPS: " + std::to_string(frameCount);

			m_fonts.ClearTextCache();

			m_fontTexture = m_fonts.GetTextTexture(txt, "Assets/Fonts/den.ttf", 18);
			if (m_fontTexture)
				SDL_QueryTexture(m_fontTexture, nullptr, nullptr, &m_fontDst.w, &m_fontDst.h);

			frameCount = 0;
			fpsTimer += 1000;
		}
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				m_isRunning = false;
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					m_isRunning = false;
				}
				break;

			case SDL_KEYUP:
				break;

			case SDL_MOUSEMOTION:
				break;

			case SDL_MOUSEBUTTONDOWN:
				std::cout << "Mouse: " << event.button.x << "," << event.button.y << std::endl;
				break;

			default:
				break;
		}
	}
}

void Game::Update(float deltaTime)
{
	m_sceneManager.Update(deltaTime);
}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 30, 30, 90, 255); // background
	SDL_RenderClear(m_renderer); // clear window
	
	m_RQ.Clear();

	// Fonts
	m_RQ.Push({ m_fontTexture, m_fontDst, SDL_Rect{0,0,0,0},200 }); // zIndex = 200
	
	m_sceneManager.Render();

	m_RQ.Flush(m_renderer);
	// Double-buffering
	SDL_RenderPresent(m_renderer); 
}

void Game::Shutdown()
{
	if (!m_isRunning && !m_window && !m_renderer) return;
	m_isRunning = false;

	m_sounds.Shutdown();
	m_fonts.Shutdown();
	m_assets.Shutdown();

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

	SDL_Quit();
}