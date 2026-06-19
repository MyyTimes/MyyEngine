#include "Renderer2D.h"
#include <iostream>
#include <cmath>

SDL_Renderer* Renderer2D::m_renderer = nullptr;

void Renderer2D::Init(SDL_Renderer* renderer)
{
	m_renderer = renderer;
}

void Renderer2D::Shutdown()
{
	m_renderer = nullptr;
}

void Renderer2D::BeginScene()
{
	// İleride Batch Rendering (Toplu Çizim) için buffer'ları (hafıza) 
	// burada sıfırlayacağız. Şimdilik anında (immediate) çizim yapıyoruz.
}

void Renderer2D::EndScene()
{
	Flush();
}

void Renderer2D::Flush()
{
	// SDL 2.0.18+ ile SDL_RenderGeometry kullanarak tüm sprite'ları 
	// tek bir draw call'da çizeceğimiz sistem buraya gelecek.
}

static SDL_BlendMode GetSDLBlendMode(BlendMode mode)
{
	switch (mode)
	{
		case BlendMode::None:     return SDL_BLENDMODE_NONE;
		case BlendMode::Blend:    return SDL_BLENDMODE_BLEND;
		case BlendMode::Additive: return SDL_BLENDMODE_ADD;
		case BlendMode::Multiply: return SDL_BLENDMODE_MOD;
		default:                  return SDL_BLENDMODE_BLEND;
	}
}

void Renderer2D::DrawSprite(SDL_Texture* texture, const Vector2f &pos, const SpriteProperties& props)
{
	if (!texture || !m_renderer) return;

	int textureW, textureH;
	SDL_QueryTexture(texture, nullptr, nullptr, &textureW, &textureH);
	SDL_Rect dstRect;
	dstRect.x = static_cast<int>(pos.x);
	dstRect.y = static_cast<int>(pos.y);
	dstRect.w = static_cast<int>(textureW * props.scale.x);
	dstRect.h = static_cast<int>(textureH * props.scale.y);

	DrawSprite(texture, { 0, 0, textureW, textureH }, dstRect, props);
}

void Renderer2D::DrawSprite(SDL_Texture* texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect, const SpriteProperties& props)
{
	if (!texture || !m_renderer) return;

	// Set blend mode (Alpha, Additive...)
	SDL_SetTextureBlendMode(texture, GetSDLBlendMode(props.blendMode));

	// Color filter
	SDL_SetTextureColorMod(texture, props.colorTint.r, props.colorTint.g, props.colorTint.b);
	SDL_SetTextureAlphaMod(texture, props.colorTint.a);

	// Draw (rotation center is its mid point)
	SDL_Point center = { dstRect.w / 2, dstRect.h / 2 };

	SDL_RenderCopyEx(m_renderer, texture, &srcRect, &dstRect, props.rotation, &center, props.flip);
}

void Renderer2D::DrawRect(const Vector2f& pos, const Vector2f& size, const SDL_Color& color, bool filled)
{
	if (!m_renderer) return;

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

	SDL_Rect rect = {
		static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		static_cast<int>(size.x),
		static_cast<int>(size.y)
	};

	if (filled)
	{
		SDL_RenderFillRect(m_renderer, &rect);
	}
	else
	{
		SDL_RenderDrawRect(m_renderer, &rect);
	}
}

void Renderer2D::DrawCircle(const Vector2f& center, float radius, const SDL_Color& color, bool filled)
{
	if (!m_renderer) return;

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

	int cx = static_cast<int>(center.x);
	int cy = static_cast<int>(center.y);
	int r = static_cast<int>(radius);

	// Midpoint Circle Algorithm
	int offsetX = 0;
	int offsetY = r;
	int d = r - 1;

	while (offsetY >= offsetX)
	{
		if (filled)
		{
			SDL_RenderDrawLine(m_renderer, cx - offsetY, cy + offsetX, cx + offsetY, cy + offsetX);
			SDL_RenderDrawLine(m_renderer, cx - offsetX, cy + offsetY, cx + offsetX, cy + offsetY);
			SDL_RenderDrawLine(m_renderer, cx - offsetX, cy - offsetY, cx + offsetX, cy - offsetY);
			SDL_RenderDrawLine(m_renderer, cx - offsetY, cy - offsetX, cx + offsetY, cy - offsetX);
		}
		else
		{
			// just put a point on the end
			SDL_RenderDrawPoint(m_renderer, cx + offsetX, cy + offsetY);
			SDL_RenderDrawPoint(m_renderer, cx + offsetY, cy + offsetX);
			SDL_RenderDrawPoint(m_renderer, cx - offsetX, cy + offsetY);
			SDL_RenderDrawPoint(m_renderer, cx - offsetY, cy + offsetX);
			SDL_RenderDrawPoint(m_renderer, cx + offsetX, cy - offsetY);
			SDL_RenderDrawPoint(m_renderer, cx + offsetY, cy - offsetX);
			SDL_RenderDrawPoint(m_renderer, cx - offsetX, cy - offsetY);
			SDL_RenderDrawPoint(m_renderer, cx - offsetY, cy - offsetX);
		}

		if (d >= 2 * offsetX)
		{
			d -= 2 * offsetX + 1;
			offsetX++;
		}
		else if (d < 2 * (r - offsetY))
		{
			d += 2 * offsetY - 1;
			offsetY--;
		}
		else
		{
			d += 2 * (offsetY - offsetX - 1);
			offsetY--;
			offsetX++;
		}
	}
}

void Renderer2D::DrawLine(const Vector2f& start, const Vector2f& end, const SDL_Color& color)
{
	if (!m_renderer) return;

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

	SDL_RenderDrawLine(m_renderer,
		static_cast<int>(start.x), static_cast<int>(start.y),
		static_cast<int>(end.x), static_cast<int>(end.y));
}

void Renderer2D::DrawText(SDL_Texture* textTexture, const Vector2f& position, const SpriteProperties& props)
{
	DrawSprite(textTexture, position, props);
}
