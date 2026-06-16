#pragma once
#include <SDL2/SDL_mixer.h>
#include <unordered_map>
#include <string>

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	// Copying and Moving is Forbidden
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;

	void Init();
	void Shutdown();
	Mix_Chunk* GetSound(const std::string&, const std::string& = "");
	void Play(const std::string&, int = 0);

private:
	std::unordered_map<std::string, Mix_Chunk*> m_cache;

};