#include "painter.hpp"

void Painter::init() {
    m_window = SDL_CreateWindow(TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    if (!m_window) { m_error = SDL_GetError(); return; }

    m_renderer = SDL_CreateRenderer(m_window, -1,
		SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC |
		SDL_RENDERER_TARGETTEXTURE
    );
	if (!m_renderer) { m_error = SDL_GetError(); return; }

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
}

void Painter::deinit() {
	if (m_renderer) {
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

	if (m_window) {
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
}

void Painter::update() {
    SDL_RenderPresent(m_renderer);
	SDL_RenderClear(m_renderer);
}

void Painter::draw_rect(Rect rect, bool filled, Color color) {
    // TODO: Push/pop color
	SDL_FRect frect = rect.to_sdl();
    Color currcolor = get_current_color();
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	if (filled) SDL_RenderFillRectF(m_renderer, &frect);
	else SDL_RenderDrawRectF(m_renderer, &frect);
    SDL_SetRenderDrawColor(m_renderer, currcolor.r, currcolor.g, currcolor.b, currcolor.a);
}

void Painter::draw_circle(Vector center, float radius, bool filled, Color color) {
    Color currcolor = get_current_color();
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

	if (filled) {
		for (int w = 0; w < radius * 2; w++) {
			for (int h = 0; h < radius * 2; h++) {
				float dx = radius - (float) w; // horizontal offset
				float dy = radius - (float) h; // vertical offset
				if ((dx*dx + dy*dy) <= (radius * radius)) {
					SDL_RenderDrawPointF(m_renderer, center.x + dx, center.y + dy);
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

void Painter::draw_line(Vector a, Vector b, Color color) {
    Color currcolor = get_current_color();
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLineF(m_renderer, a.x, a.y, b.x, b.y);
    SDL_SetRenderDrawColor(m_renderer, currcolor.r, currcolor.g, currcolor.b, currcolor.a);
}

void Painter::draw_point(Vector point, Color color) {
    Color currcolor = get_current_color();
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPointF(m_renderer, point.x, point.y);
    SDL_SetRenderDrawColor(m_renderer, currcolor.r, currcolor.g, currcolor.b, currcolor.a);
}

void Painter::draw_texture(Rect dest, Texture* tex) {
	SDL_FRect dest2 = dest.to_sdl();
	SDL_RenderCopyF(m_renderer, tex->get_sdl_texture(), NULL, &dest2);
}

void Painter::draw_texture(Rect dest, Texture* tex, float angle, Vector center) {
	SDL_FRect dest2 = {dest.pos.x-(dest.size.x/2), dest.pos.y-(dest.size.y/2), dest.size.x, dest.size.y};
	SDL_FPoint fpoint = dest.pos.to_sdl();
	SDL_RenderCopyExF(m_renderer, tex->get_sdl_texture(), NULL, &dest2, (double) angle, &fpoint, SDL_FLIP_NONE);
}

Color Painter::get_current_color() {
    Color resp;
    SDL_GetRenderDrawColor(m_renderer, &resp.r, &resp.g, &resp.b, &resp.a);
    return resp;
}
