#include <iostream>
#include <SDL2/SDL.h>

#include "Game.h"

#include "../Systems/InputSystem.h"
#include "../Core/Logger.h"

constexpr float FIXED_DT = 1.0f / 60.0f;  // Fixed timestep for physics
constexpr float MAX_DT = 0.05f;            // Cap delta time to prevent spiral of death

Game::Game() {}
Game::~Game() { Shutdown(); }

bool Game::Init(const std::string& title, int width, int height, bool isVSync)
{
	Logger::Init();

	// SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		std::cerr << "SDL_Init Failed: " << SDL_GetError() << std::endl;
		return false;
	}

	// Window
	if (!m_window.Init(title, width, height, isVSync))
	{
		SDL_Quit();
		return false;
	}
	SDL_Renderer* m_renderer = m_window.GetRenderer();

	Renderer2D::Init(m_window.GetRenderer());

	// Textures and Asset Manager
	m_assets.Init(m_renderer);

	// Input system -> instance
	InputSystem::GetInstance().AddSubscriber([this](const SDL_Event& event)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
			{
				m_isRunning = false;
			}
		}
	);

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
	m_sceneManager.LoadScene(std::make_unique<DemoGame>(m_renderer, &m_RQ, &m_assets, &m_sounds));

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
	InputSystem::GetInstance().Update();
}

void Game::Update(float deltaTime)
{
	m_sceneManager.Update(deltaTime);
}

void Game::Render()
{
	m_window.Clear(30, 30, 90, 255); // background
	
	m_RQ.Clear();

	// Fonts
	m_RQ.Push({ m_fontTexture, m_fontDst, SDL_Rect{0,0,0,0},200 }); // zIndex = 200
	
	m_sceneManager.Render();

	m_RQ.Flush();
	// Double-buffering

	m_window.Present();
}

void Game::Shutdown()
{
	if (m_isShuttingDown) return;
	m_isShuttingDown = true;
	m_isRunning = false;

	m_sounds.Shutdown();
	m_fonts.Shutdown();
	m_assets.Shutdown();
	m_window.Shutdown();

	Renderer2D::Shutdown();

	SDL_Quit();

	Logger::Shutdown();
}
