// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "graphicsengine.hpp"

#include <optional>

#include "cog2d/program.hpp"

namespace cog2d {

void GraphicsEngine::init(ProgramSettings* settings)
{
	m_window = SDL_CreateWindow(settings->title.data(), SDL_WINDOWPOS_CENTERED,
	                            SDL_WINDOWPOS_CENTERED, settings->wwidth, settings->wheight,
	                            settings->window_flags);
	if (!m_window) {
		m_error = SDL_GetError();
		return;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, settings->render_flags);
	if (!m_renderer) {
		m_error = SDL_GetError();
		return;
	}

	SDL_SetRenderDrawBlendMode(m_renderer, settings->blend_mode);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, settings->scale_quality.data());
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, settings->vsync ? "1" : "0");
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, settings->batching ? "1" : "0");

	SDL_Rect viewport = {0, 0, settings->lwidth, settings->lheight};
	SDL_RenderSetViewport(m_renderer, &viewport);
	SDL_RenderSetLogicalSize(m_renderer, settings->lwidth, settings->lheight);
	SDL_SetWindowMinimumSize(m_window, settings->lwidth, settings->lheight);

	m_logical_size.x = settings->lwidth;
	m_logical_size.y = settings->lheight;

	if (settings->proxy_texture) {
		m_proxy = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
		                            settings->lwidth, settings->lheight);
	}
}

void GraphicsEngine::deinit()
{
	if (m_proxy) {
		SDL_DestroyTexture(m_proxy);
		m_proxy = nullptr;
	}

	if (m_renderer) {
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

	if (m_window) {
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
}

void GraphicsEngine::pre_draw()
{
	SDL_SetRenderDrawColor(get_renderer(), 0x0, 0x0, 0x0, 0x0);

	if (m_proxy)
		SDL_SetRenderTarget(m_renderer, m_proxy);

	SDL_RenderClear(m_renderer);
}

void GraphicsEngine::post_draw()
{
	SDL_SetRenderTarget(m_renderer, nullptr);

	if (m_proxy)
		SDL_RenderCopy(m_renderer, m_proxy, nullptr, nullptr);

	SDL_RenderPresent(m_renderer);
}

void GraphicsEngine::draw_rect(Rect rect, bool filled, Color color)
{
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

#ifdef COG2D_GRAPHICS_USE_INT
	SDL_Rect srect = rect.to_sdl_rect();
#else
	SDL_FRect srect = rect.to_sdl_frect();
#endif

#ifdef COG2D_GRAPHICS_USE_INT
	if (filled)
		SDL_RenderFillRect(m_renderer, &srect);
	else
		SDL_RenderDrawRect(m_renderer, &srect);
#else
	if (filled)
		SDL_RenderFillRectF(m_renderer, &srect);
	else
		SDL_RenderDrawRectF(m_renderer, &srect);
#endif

	//SDL_SetRenderDrawColor(get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
}

void GraphicsEngine::draw_circle(Vector center, float radius, bool filled, Color color)
{
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

	if (filled) {
		for (int w = 0; w < radius * 2; w++) {
			for (int h = 0; h < radius * 2; h++) {
				float dx = radius - (float) w;  // horizontal offset
				float dy = radius - (float) h;  // vertical offset
				if ((dx * dx + dy * dy) <= (radius * radius)) {
#ifdef COG2D_GRAPHICS_USE_INT
					SDL_RenderDrawPoint(m_renderer, static_cast<int>(center.x + dx),
					                    static_cast<int>(center.y + dy));
#else
					SDL_RenderDrawPointF(m_renderer, center.x + dx, center.y + dy);
#endif
				}
			}
		}
	} else {
		int diameter = (((int) radius) << 1);
		int x = ((int) radius) - 1;
		int y = 0;
		int dx = 1;
		int dy = 1;
		int err = dx - diameter;
		int centerX = (int) center.x;
		int centerY = (int) center.y;

		while (x >= y) {
			SDL_RenderDrawPoint(m_renderer, centerX + x, centerY + y);
			SDL_RenderDrawPoint(m_renderer, centerX + y, centerY + x);
			SDL_RenderDrawPoint(m_renderer, centerX - y, centerY + x);
			SDL_RenderDrawPoint(m_renderer, centerX - x, centerY + y);
			SDL_RenderDrawPoint(m_renderer, centerX - x, centerY - y);
			SDL_RenderDrawPoint(m_renderer, centerX - y, centerY - x);
			SDL_RenderDrawPoint(m_renderer, centerX + y, centerY - x);
			SDL_RenderDrawPoint(m_renderer, centerX + x, centerY - y);

			if (err <= 0) {
				y++;
				err += dy;
				dy += 2;
			}

			if (err > 0) {
				x--;
				dx += 2;
				err += dx - diameter;
			}
		}
	}

	//SDL_SetRenderDrawColor(get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
}

void GraphicsEngine::draw_line(Vector a, Vector b, Color color)
{
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

#ifdef COG2D_GRAPHICS_USE_INT
	SDL_RenderDrawLine(m_renderer, static_cast<int>(a.x), static_cast<int>(a.y),
	                   static_cast<int>(b.x), static_cast<int>(b.y));
#else
	SDL_RenderDrawLineF(m_renderer, a.x, a.y, b.x, b.y);
#endif

	//SDL_SetRenderDrawColor(get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
}

void GraphicsEngine::draw_point(Vector point, Color color)
{
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

#ifdef COG2D_GRAPHICS_USE_INT
	SDL_RenderDrawPoint(m_renderer, static_cast<int>(point.x), static_cast<int>(point.y));
#else
	SDL_RenderDrawPointF(m_renderer, point.x, point.y);
#endif

	//SDL_SetRenderDrawColor(get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
}

void GraphicsEngine::draw_texture(Texture* tex, Rect_t<int> src, Rect dest, Color color,
                                  float angle, Vector center, SDL_RendererFlip flip)
{
	SDL_SetRenderDrawColor(get_renderer(), color.r, color.g, color.b, color.a);

#ifdef COG2D_GRAPHICS_USE_INT
	SDL_Point scenter = center.to_sdl_point();
	if (scenter.x < 0)
		scenter.x = tex->get_size().x / 2;

	if (scenter.y < 0)
		scenter.y = tex->get_size().y / 2;

	SDL_Rect ssrc = src.to_sdl_rect();
	SDL_Rect sdest = dest.to_sdl_rect();
	SDL_RenderCopyEx(m_renderer, tex->to_sdl(), &ssrc, &sdest, static_cast<float>(angle), &scenter,
	                 flip);
#else
	SDL_FPoint scenter = center.to_sdl_fpoint();
	if (scenter.x < 0) {
		scenter.x = tex->get_size().x / 2;
	}

	if (scenter.y < 0) {
		scenter.y = tex->get_size().y / 2;
	}

	SDL_Rect ssrc = src.to_sdl_rect();
	SDL_FRect sdest = dest.to_sdl_frect();
	SDL_RenderCopyExF(m_renderer, tex->to_sdl(), &ssrc, &sdest, static_cast<float>(angle), &scenter,
	                  flip);
#endif

	//SDL_SetRenderDrawColor(get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);
}

void GraphicsEngine::draw_texture(Texture* tex, Rect dest, Color color, float angle, Vector center,
                                  SDL_RendererFlip flip)
{
	draw_texture(tex, {{0, 0}, tex->get_size()}, dest, color, angle, center, flip);
}

void GraphicsEngine::draw_texture(Texture* tex, Vector pos, Color color, float angle, Vector center,
                                  SDL_RendererFlip flip)
{
	draw_texture(tex, {pos, tex->get_size()}, color, angle, center, flip);
}

Color GraphicsEngine::get_current_color()
{
	Color resp;
	SDL_GetRenderDrawColor(m_renderer, &resp.r, &resp.g, &resp.b, &resp.a);
	return resp;
}

void GraphicsEngine::push_target(Texture* tex)
{
	m_target_stack.push(tex);
	SDL_SetRenderTarget(get_renderer(), m_target_stack.empty() ? nullptr : get_target()->to_sdl());
}

void GraphicsEngine::pop_target()
{
	m_target_stack.pop();
	SDL_SetRenderTarget(get_renderer(), m_target_stack.empty() ? nullptr : get_target()->to_sdl());
}
}  //namespace cog2d
