#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() {}
SoundManager::~SoundManager() { Shutdown(); }

void SoundManager::Init()
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void SoundManager::Shutdown()
{
	if (m_cache.empty()) return; // çift çağrı koruması

	for (auto& kv : m_cache)
	{
		if (kv.second)
			Mix_FreeChunk(kv.second);
	}
	m_cache.clear();
	Mix_CloseAudio();
}

Mix_Chunk* SoundManager::GetSound(const std::string &key, const std::string &path)
{
	auto it = m_cache.find(key); 
	if (it != m_cache.end()) return it->second;
	Mix_Chunk* chunk = Mix_LoadWAV(path.empty() ? key.c_str() : path.c_str());
	if (!chunk)
	{
		std::cerr << Mix_GetError() << std::endl;
		return nullptr;
	}
	m_cache[key] = chunk;
	return chunk;
}

void SoundManager::Play(const std::string &key, int loops)
{
	Mix_PlayChannel(-1, GetSound(key), loops);
}