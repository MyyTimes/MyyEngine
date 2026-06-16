#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <string>

struct FontKey
{
	std::string path;
	int size;
};

inline bool operator==(const FontKey& a, const FontKey& b)
{
	return a.path == b.path && a.size == b.size;
}

struct FontKeyHash
{
	size_t operator()(const FontKey& k) const noexcept
	{
		return std::hash<std::string>{}(k.path) ^ (k.size << 1);
	}
};

class FontManager
{
public:
	FontManager();
	~FontManager();

	// Copying and Moving is Forbidden
	FontManager(const FontManager&) = delete;
	FontManager& operator=(const FontManager&) = delete;
	FontManager(FontManager&&) = delete;
	FontManager& operator=(FontManager&&) = delete;

	void Init(SDL_Renderer*);
	void Shutdown();
	TTF_Font* GetFont(const std::string&, int);
	SDL_Texture* GetTextTexture(const std::string&, const std::string&, int, SDL_Color = { 255,255,255,255 });
	void ClearTextCache(); // for dynamic text ( FPS etc. )

private:
	SDL_Renderer* m_renderer = nullptr;
	std::unordered_map<FontKey, TTF_Font*, FontKeyHash> m_fonts;
	std::unordered_map<std::string, SDL_Texture*> m_textCache;
};