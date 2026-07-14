#include "Renderer2D.h"
#include <iostream>
#include <cmath>

void Renderer2D::Init(SDL_Renderer* renderer)
{
	m_renderer = renderer;
	// batch rendering
	s_vertices.reserve(MAX_VERTICES);
	s_indices.reserve(MAX_INDICES);
}

void Renderer2D::Shutdown()
{
	m_renderer = nullptr;
	s_vertices.clear();
	s_indices.clear();
}

void Renderer2D::BeginScene()
{
	// batch rendering
	s_quadCount = 0;
	s_vertices.clear();
	s_indices.clear();
	s_currTexture = nullptr;
	s_currBlendMode = SDL_BLENDMODE_BLEND;
}

void Renderer2D::EndScene()
{
	Flush();
}

void Renderer2D::Flush()
{
	if (s_quadCount == 0) return;

	if (s_currTexture)
	{
		SDL_SetTextureBlendMode(s_currTexture, s_currBlendMode);
	}

	// send all sprites to GPU to render
	SDL_RenderGeometry(m_renderer, s_currTexture, s_vertices.data(), s_vertices.size(), s_indices.data(), s_indices.size());

	s_quadCount = 0;
	s_vertices.clear();
	s_indices.clear();
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
	if (!texture) return;

	int texW, texH;
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);

	SDL_Rect srcRect = { 0, 0, texW, texH };

	SDL_Rect dstRect = {
		static_cast<int>(pos.x - (texW / 2.0f)),
		static_cast<int>(pos.y - (texH / 2.0f)),
		texW,
		texH
	};

	DrawSprite(texture, srcRect, dstRect, props);


	/*
	if (!texture || !m_renderer) return;

	int textureW, textureH;
	SDL_QueryTexture(texture, nullptr, nullptr, &textureW, &textureH);
	SDL_Rect dstRect;
	dstRect.x = static_cast<int>(pos.x);
	dstRect.y = static_cast<int>(pos.y);
	dstRect.w = static_cast<int>(textureW * props.scale.x);
	dstRect.h = static_cast<int>(textureH * props.scale.y);

	DrawSprite(texture, { 0, 0, textureW, textureH }, dstRect, props);
	*/
}

void Renderer2D::DrawSprite(SDL_Texture* texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect, const SpriteProperties& props)
{
	SDL_BlendMode targetBlendMode = GetSDLBlendMode(props.blendMode);

	if (s_quadCount >= MAX_QUADS || (s_currTexture != texture && s_currTexture != nullptr) || (s_currBlendMode != targetBlendMode && s_quadCount > 0))
	{
		Flush();
	}

	s_currTexture = texture;
	s_currBlendMode = targetBlendMode;

	int texW, texH;
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);

	// calculate UV coordinates (0.0 - 1.0)
	float u_min = static_cast<float>(srcRect.x) / texW;
	float v_min = static_cast<float>(srcRect.y) / texH;
	float u_max = static_cast<float>(srcRect.x + srcRect.w) / texW;
	float v_max = static_cast<float>(srcRect.y + srcRect.h) / texH;

	// flipping
	if (props.flip & SDL_FLIP_HORIZONTAL) std::swap(u_min, u_max);
	if (props.flip & SDL_FLIP_VERTICAL) std::swap(v_min, v_max);

	// calculate center and size
	float cx = dstRect.x + (dstRect.w / 2.0f);
	float cy = dstRect.y + (dstRect.h / 2.0f);
	float halfW = (dstRect.w * props.scale.x) / 2.0f;
	float halfH = (dstRect.h * props.scale.y) / 2.0f;

	// rotation
	float rad = props.rotation * (M_PI / 180.0f);
	float cosA = std::cos(rad);
	float sinA = std::sin(rad);

	// rotate the point according to center point
	auto RotatePoint = [&](float x, float y) -> SDL_FPoint {
		return {
			cx + (x * cosA - y * sinA),
			cy + (x * sinA + y * cosA)
		};
		};

	SDL_FPoint p0 = RotatePoint(-halfW, -halfH); // LT
	SDL_FPoint p1 = RotatePoint(halfW, -halfH);  // RT
	SDL_FPoint p2 = RotatePoint(halfW, halfH);   // RB
	SDL_FPoint p3 = RotatePoint(-halfW, halfH);  // LB

	SDL_Color color = props.colorTint;

	s_vertices.push_back({ p0, color, {u_min, v_min} });
	s_vertices.push_back({ p1, color, {u_max, v_min} });
	s_vertices.push_back({ p2, color, {u_max, v_max} });
	s_vertices.push_back({ p3, color, {u_min, v_max} });

	int offset = s_quadCount * 4;
	s_indices.push_back(offset + 0);
	s_indices.push_back(offset + 1);
	s_indices.push_back(offset + 2);
	s_indices.push_back(offset + 2);
	s_indices.push_back(offset + 3);
	s_indices.push_back(offset + 0);

	s_quadCount++;

	/*
	if (!texture || !m_renderer) return;

	// Set blend mode (Alpha, Additive...)
	SDL_SetTextureBlendMode(texture, GetSDLBlendMode(props.blendMode));

	// Color filter
	SDL_SetTextureColorMod(texture, props.colorTint.r, props.colorTint.g, props.colorTint.b);
	SDL_SetTextureAlphaMod(texture, props.colorTint.a);

	// Draw (rotation center is its mid point)
	SDL_Point center = { dstRect.w / 2, dstRect.h / 2 };

	SDL_RenderCopyEx(m_renderer, texture, &srcRect, &dstRect, props.rotation, &center, props.flip);
	*/
}

void Renderer2D::DrawRect(const Vector2f& pos, const Vector2f& size, const SDL_Color& color, bool filled)
{
	Flush();

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
	Flush();

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
	Flush();

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

