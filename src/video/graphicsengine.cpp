#include "graphicsengine.hpp"

#include "cog2d/program.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

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
	SDL_RenderClear(m_renderer);

	if (m_proxy) {
		SDL_SetRenderTarget(m_renderer, m_proxy);
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0xFF);
		SDL_Rect rect = Rect_t<int>({0, 0}, get_logical_size()).to_sdl_rect();
		SDL_RenderFillRect(m_renderer, &rect);
	}
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
	// TODO: Push/pop color
#ifdef COG2D_GRAPHICS_USE_INT
	SDL_Rect srect = rect.to_sdl_rect();
#else
	SDL_FRect srect = rect.to_sdl_frect();
#endif

	Color currcolor = get_current_color();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

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

	SDL_SetRenderDrawColor(m_renderer, currcolor.r, currcolor.g, currcolor.b, currcolor.a);
}

void GraphicsEngine::draw_circle(Vector center, float radius, bool filled, Color color)
{
	Color currcolor = get_current_color();
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

	SDL_SetRenderDrawColor(m_renderer, currcolor.r, currcolor.g, currcolor.b, currcolor.a);
}

void GraphicsEngine::draw_line(Vector a, Vector b, Color color)
{
	Color currcolor = get_current_color();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
#ifdef COG2D_GRAPHICS_USE_INT
	SDL_RenderDrawLine(m_renderer, static_cast<int>(a.x), static_cast<int>(a.y),
	                   static_cast<int>(b.x), static_cast<int>(b.y));
#else
	SDL_RenderDrawLineF(m_renderer, a.x, a.y, b.x, b.y);
#endif
	SDL_SetRenderDrawColor(m_renderer, currcolor.r, currcolor.g, currcolor.b, currcolor.a);
}

void GraphicsEngine::draw_point(Vector point, Color color)
{
	Color currcolor = get_current_color();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
#ifdef COG2D_GRAPHICS_USE_INT
	SDL_RenderDrawPoint(m_renderer, static_cast<int>(point.x), static_cast<int>(point.y));
#else
	SDL_RenderDrawPointF(m_renderer, point.x, point.y);
#endif
	SDL_SetRenderDrawColor(m_renderer, currcolor.r, currcolor.g, currcolor.b, currcolor.a);
}

void GraphicsEngine::draw_texture(Rect dest, Texture* tex)
{
	if (dest.size.x < 0) {
		dest.size.x = tex->m_size.x;
	}

	if (dest.size.y < 0) {
		dest.size.y = tex->m_size.y;
	}

#ifdef COG2D_GRAPHICS_USE_INT
	SDL_Rect dest2 = dest.to_sdl_rect();
	SDL_RenderCopy(m_renderer, tex->to_sdl(), NULL, &dest2);
#else
	SDL_FRect dest2 = dest.to_sdl_frect();
	SDL_RenderCopyF(m_renderer, tex->to_sdl(), NULL, &dest2);
#endif
}

void GraphicsEngine::draw_texture(Rect dest, Texture* tex, float angle, SDL_RendererFlip flip)
{
#ifdef COG2D_GRAPHICS_USE_INT
	SDL_Rect dest2 = {static_cast<int>(dest.pos.x - (dest.size.x / 2)),
	                  static_cast<int>(dest.pos.y - (dest.size.y / 2)),
	                  static_cast<int>(dest.size.x), static_cast<int>(dest.size.y)};
	SDL_Point fpoint = dest.pos.to_sdl_point();
	SDL_RenderCopyEx(m_renderer, tex->to_sdl(), NULL, &dest2, (double) angle, &fpoint,
	                 SDL_FLIP_NONE);
#else
	SDL_FRect dest2 = {
	    dest.pos.x - (dest.size.x / 2), dest.pos.y - (dest.size.y / 2), dest.size.x, dest.size.y};
	SDL_FPoint fpoint = dest.pos.to_sdl_fpoint();
	SDL_RenderCopyExF(m_renderer, tex->to_sdl(), NULL, &dest2, (double) angle, &fpoint,
	                  flip);
#endif
}

Color GraphicsEngine::get_current_color()
{
	Color resp;
	SDL_GetRenderDrawColor(m_renderer, &resp.r, &resp.g, &resp.b, &resp.a);
	return resp;
}

COG2D_NAMESPACE_END_IMPL
