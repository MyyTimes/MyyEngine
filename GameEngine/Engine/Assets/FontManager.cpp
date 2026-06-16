#include "FontManager.h"
#include <iostream>

FontManager::FontManager() {}
FontManager::~FontManager() { Shutdown(); }

void FontManager::Init(SDL_Renderer* r)
{
	m_renderer = r;
	TTF_Init();
}

TTF_Font* FontManager::GetFont(const std::string& path, int pt)
{
	FontKey k{ path,pt };
	if (auto it = m_fonts.find(k); it != m_fonts.end()) return it->second;

	TTF_Font* f = TTF_OpenFont(path.c_str(), pt);
	if (!f)
	{
		std::cerr << TTF_GetError() << std::endl;
		return nullptr;
	}

	m_fonts[k] = f;
	return f;
}

SDL_Texture* FontManager::GetTextTexture(const std::string& txt, const std::string& fontPath, int pt, SDL_Color color)
{
	const std::string cacheKey = txt + "|" + fontPath + "|" + std::to_string(pt);

	// Return if already cached
	if (auto it = m_textCache.find(cacheKey); it != m_textCache.end()) return it->second;

	TTF_Font* font = GetFont(fontPath, pt);
	if (!font) return nullptr;

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, txt.c_str(), color);
	if (!surface) return nullptr;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_FreeSurface(surface);

	m_textCache[cacheKey] = texture;
	return texture;
}

void FontManager::ClearTextCache()
{
	for (auto& kv : m_textCache)
	{
		if (kv.second)
			SDL_DestroyTexture(kv.second);
	}
	m_textCache.clear();
}

void FontManager::Shutdown()
{
	if (m_textCache.empty() && m_fonts.empty() && !m_renderer) return; 
	
	for (auto& kv : m_textCache)
	{
		if (kv.second)
			SDL_DestroyTexture(kv.second);
	}
	for (auto& kv : m_fonts)
	{
		if (kv.second)
			TTF_CloseFont(kv.second);
	}
	m_textCache.clear();
	m_fonts.clear();

	TTF_Quit();
	m_renderer = nullptr;
}