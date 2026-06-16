#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <unordered_map>
#include <string>

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	// Copying and Moving is Forbidden
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;
	AssetManager(AssetManager&&) = delete;
	AssetManager& operator=(AssetManager&&) = delete;

	void Init(SDL_Renderer*);
	void Shutdown();

	SDL_Texture* GetTexture(const std::string&, const std::string& = "");
	//Mix_Chunk* GetSound(const std::string&); sound manager içinde

	//void LoadManifest(const std::string&);

	// ADD CREATE_ANIM_CLIP AND GET_ANIM_CLIP
private:
	SDL_Renderer* m_renderer = nullptr;
	std::unordered_map<std::string, SDL_Texture*> m_texture;
	std::unordered_map<std::string, Mix_Chunk*> m_sound;
};