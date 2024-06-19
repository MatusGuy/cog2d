#ifndef PAINTER_H
#define PAINTER_H

#include <string>
#include <cmath>
#include <SDL2/SDL.h>

#include "texture.hpp"

#define TITLE "cog2d"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

class Texture;

#define COG2D_USE_PAINTER COG2D_USING(Painter, painter)
class Painter : public Singleton<Painter> {
public:
    void init();
    void deinit();

    void update();

	void draw_rect(Rect rect, bool filled = false, Color color = 0xFFFFFFFF);
    void draw_circle(Vector center, float radius, bool filled = false, Color color = 0xFFFFFFFF);
    void draw_line(Vector a, Vector b, Color color = 0xFFFFFFFF);
    void draw_point(Vector point, Color color = 0xFFFFFFFF);
	void draw_texture(Rect dest, Texture* tex);
	void draw_texture(Rect dest, Texture* tex, float angle, Vector center = {0,0});

	inline SDL_Window* get_window() { return m_window; }
	inline SDL_Renderer* get_renderer() { return m_renderer; }

    Color get_current_color();

    inline const std::string& get_error() { return m_error; }

private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

    std::string m_error = "";
};

#endif // PAINTER_H
