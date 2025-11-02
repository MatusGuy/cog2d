// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "sdl2graphicsengine.hpp"

#include <optional>

#include "cog2d/program.hpp"
#include "cog2d/util/logger.hpp"

namespace cog2d::graphics {

static struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* proxy;
	std::stack<Texture*> target_stack;
	Vector_t<int> logical_size;
} s_engine;

void init(ProgramSettings& settings)
{
	pixmap_backend = Backend::PIXMAP_SDL_IMAGE;
	ttf_backend = Backend::TTF_SDL_TTF;

	int init = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	if (init != 0) {
		log::fatal("SDL2GraphicsEngine",
		           fmt::format("SDL2 failed to initialize video & events with "
		                       "errcode {} and reason: '{}'",
		                       init, SDL_GetError()));
		return;
	}

	s_engine.window = SDL_CreateWindow(settings.title.data(), SDL_WINDOWPOS_CENTERED,
	                                   SDL_WINDOWPOS_CENTERED, settings.wwidth, settings.wheight,
	                                   settings.window_flags);
	if (!s_engine.window) {
		log::fatal("SDL2GraphicsEngine", fmt::format("SDL2 failed to create window with "
		                                             "reason: '{}'",
		                                             SDL_GetError()));
		return;
	}

	s_engine.renderer = SDL_CreateRenderer(s_engine.window, -1, settings.render_flags);
	if (!s_engine.renderer) {
		log::fatal("SDL2GraphicsEngine", fmt::format("SDL2 failed to create renderer with "
		                                             "reason: '{}'",
		                                             SDL_GetError()));
		return;
	}

	SDL_SetRenderDrawBlendMode(s_engine.renderer, settings.blend_mode);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, settings.scale_quality.data());
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, settings.vsync ? "1" : "0");
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, settings.batching ? "1" : "0");

	SDL_Rect viewport = {0, 0, settings.lwidth, settings.lheight};
	SDL_RenderSetViewport(s_engine.renderer, &viewport);
	SDL_RenderSetLogicalSize(s_engine.renderer, settings.lwidth, settings.lheight);
	SDL_SetWindowMinimumSize(s_engine.window, settings.lwidth, settings.lheight);

	s_engine.logical_size.x = settings.lwidth;
	s_engine.logical_size.y = settings.lheight;

	if (settings.proxy_texture) {
		s_engine.proxy = SDL_CreateTexture(s_engine.renderer, SDL_PIXELFORMAT_RGBA8888,
		                                   SDL_TEXTUREACCESS_TARGET, settings.lwidth,
		                                   settings.lheight);
	}
}

void deinit()
{
	if (s_engine.proxy) {
		SDL_DestroyTexture(s_engine.proxy);
		s_engine.proxy = nullptr;
	}

	if (s_engine.renderer) {
		SDL_DestroyRenderer(s_engine.renderer);
		s_engine.renderer = nullptr;
	}

	if (s_engine.window) {
		SDL_DestroyWindow(s_engine.window);
		s_engine.window = nullptr;
	}

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void pre_draw()
{
	SDL_SetRenderDrawColor(sdl2::get_renderer(), 0x0, 0x0, 0x0, 0x0);

	if (s_engine.proxy)
		SDL_SetRenderTarget(s_engine.renderer, s_engine.proxy);

	SDL_RenderClear(s_engine.renderer);
}

void post_draw()
{
	SDL_SetRenderTarget(s_engine.renderer, nullptr);

	if (s_engine.proxy)
		SDL_RenderCopy(s_engine.renderer, s_engine.proxy, nullptr, nullptr);

	SDL_RenderPresent(s_engine.renderer);
}

void draw_rect(Rect rect, bool filled, Color color)
{
	SDL_SetRenderDrawColor(s_engine.renderer, color.r, color.g, color.b, color.a);

#ifdef COG2D_GRAPHICS_USE_INT
	SDL_Rect srect = rect.to_sdl_rect();
#else
	SDL_FRect srect = rect.to_sdl_frect();
#endif

#ifdef COG2D_GRAPHICS_USE_INT
	if (filled)
		SDL_RenderFillRect(s_engine.renderer, &srect);
	else
		SDL_RenderDrawRect(s_engine.renderer, &srect);
#else
	if (filled)
		SDL_RenderFillRectF(s_engine.renderer, &srect);
	else
		SDL_RenderDrawRectF(s_engine.renderer, &srect);
#endif
}

void draw_circle(Vector center, float radius, bool filled, Color color)
{
	SDL_SetRenderDrawColor(s_engine.renderer, color.r, color.g, color.b, color.a);

	if (filled) {
		for (int w = 0; w < radius * 2; w++) {
			for (int h = 0; h < radius * 2; h++) {
				float dx = radius - (float) w;  // horizontal offset
				float dy = radius - (float) h;  // vertical offset
				if ((dx * dx + dy * dy) <= (radius * radius)) {
#ifdef COG2D_GRAPHICS_USE_INT
					SDL_RenderDrawPoint(s_engine.renderer, static_cast<int>(center.x + dx),
					                    static_cast<int>(center.y + dy));
#else
					SDL_RenderDrawPointF(s_engine.renderer, center.x + dx, center.y + dy);
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
			SDL_RenderDrawPoint(s_engine.renderer, centerX + x, centerY + y);
			SDL_RenderDrawPoint(s_engine.renderer, centerX + y, centerY + x);
			SDL_RenderDrawPoint(s_engine.renderer, centerX - y, centerY + x);
			SDL_RenderDrawPoint(s_engine.renderer, centerX - x, centerY + y);
			SDL_RenderDrawPoint(s_engine.renderer, centerX - x, centerY - y);
			SDL_RenderDrawPoint(s_engine.renderer, centerX - y, centerY - x);
			SDL_RenderDrawPoint(s_engine.renderer, centerX + y, centerY - x);
			SDL_RenderDrawPoint(s_engine.renderer, centerX + x, centerY - y);

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
}

void draw_line(Vector a, Vector b, Color color)
{
	SDL_SetRenderDrawColor(s_engine.renderer, color.r, color.g, color.b, color.a);

#ifdef COG2D_GRAPHICS_USE_INT
	SDL_RenderDrawLine(s_engine.renderer, static_cast<int>(a.x), static_cast<int>(a.y),
	                   static_cast<int>(b.x), static_cast<int>(b.y));
#else
	SDL_RenderDrawLineF(s_engine.renderer, a.x, a.y, b.x, b.y);
#endif
}

void draw_point(Vector point, Color color)
{
	SDL_SetRenderDrawColor(s_engine.renderer, color.r, color.g, color.b, color.a);

#ifdef COG2D_GRAPHICS_USE_INT
	SDL_RenderDrawPoint(s_engine.renderer, static_cast<int>(point.x), static_cast<int>(point.y));
#else
	SDL_RenderDrawPointF(s_engine.renderer, point.x, point.y);
#endif
}

void draw_texture(Texture* tex, Rect_t<int> src, Rect dest, Color color, Flip flip, float angle,
                  Vector center)
{
	SDL_SetRenderDrawColor(sdl2::get_renderer(), color.r, color.g, color.b, color.a);

#ifdef COG2D_GRAPHICS_USE_INT
	SDL_Point scenter = center.to_sdl_point();
	if (scenter.x < 0)
		scenter.x = tex->size().x / 2;

	if (scenter.y < 0)
		scenter.y = tex->size().y / 2;

	SDL_Rect ssrc = src.to_sdl_rect();
	SDL_Rect sdest = dest.to_sdl_rect();
	SDL_RenderCopyEx(s_engine.renderer, tex->data(), &ssrc, &sdest, static_cast<float>(angle),
	                 &scenter, static_cast<SDL_RendererFlip>(flip));
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
	SDL_RenderCopyExF(s_engine.renderer, tex->data(), &ssrc, &sdest, static_cast<float>(angle),
	                  &scenter, flip);
#endif
}

Color get_current_color()
{
	Color resp;
	SDL_GetRenderDrawColor(s_engine.renderer, &resp.r, &resp.g, &resp.b, &resp.a);
	return resp;
}

void push_target(Texture* tex)
{
	s_engine.target_stack.push(tex);
	SDL_SetRenderTarget(sdl2::get_renderer(),
	                    s_engine.target_stack.empty() ? nullptr : get_target()->data());
}

void pop_target()
{
	s_engine.target_stack.pop();
	SDL_SetRenderTarget(sdl2::get_renderer(),
	                    s_engine.target_stack.empty() ? nullptr : get_target()->data());
}

Vector_t<int> logical_size()
{
	return s_engine.logical_size;
}

Texture* get_target()
{
	return s_engine.target_stack.top();
}

namespace sdl2 {

SDL_Window* get_window()
{
	return s_engine.window;
}

SDL_Renderer* get_renderer()
{
	return s_engine.renderer;
}

}  //namespace sdl2
}  //namespace cog2d::graphics
