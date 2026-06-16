#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../Core/RenderQueue.h"

#include "../Assets/SoundManager.h"
#include "../Assets/AssetManager.h"
#include "../Assets/FontManager.h"

#include "../Core/Math/Vector2.h"

#include "../Scene/SceneManager.h"

#include "DemoGame.h"

class Game
{
public:
	Game();
	~Game();

	// Copying and Moving is Forbidden
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

	bool Init(const std::string&, int, int);
	void Run();
	void Shutdown();

private:
	// Functions
	void ProcessInput();
	void Update(float);
	void Render();

	// Variables
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	bool m_isRunning = false;
	int m_windowWidth = 800;
	int m_windowHeight = 800;

	// Scene Manager
	SceneManager m_sceneManager;

	// Global Managers
	RenderQueue m_RQ;
	AssetManager m_assets;
	FontManager m_fonts;
	SoundManager m_sounds;

	// for FPS
	SDL_Texture* m_fontTexture = nullptr;
	SDL_Rect m_fontDst{ 10,10,0,0 };
};
