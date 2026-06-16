#include "AssetManager.h"
#include <iostream>
#include <filesystem>

AssetManager::AssetManager() {}
AssetManager::~AssetManager() { Shutdown(); }

void AssetManager::Init(SDL_Renderer* r)
{
	m_renderer = r;
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}

void AssetManager::Shutdown()
{
	if (m_texture.empty() && !m_renderer) return;

	for (auto& kv : m_texture)
	{
		if (kv.second)
			SDL_DestroyTexture(kv.second);
	}
	m_texture.clear();

	IMG_Quit();
	m_renderer = nullptr;
}

SDL_Texture* AssetManager::GetTexture(const std::string &key, const std::string &path)
{
	if (auto it = m_texture.find(key); it != m_texture.end())
	{
		return it->second;
	}

	const std::string file = path.empty() ? key : path;

	SDL_Surface* surface = IMG_Load(file.c_str());
	if (!surface)
	{
		std::cerr << IMG_GetError() << std::endl;
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture)
	{
		std::cerr << SDL_GetError() << std::endl;
		return nullptr;
	}

	m_texture[key] = texture;
	return texture;
}