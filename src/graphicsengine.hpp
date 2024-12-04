#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include <string>
#include <cmath>
#include <SDL2/SDL.h>

#include "texture.hpp"
#include "font.hpp"

struct ProgramSettings;

#define COG2D_USE_GRAPHICSENGINE COG2D_USING(GraphicsEngine, graphicsengine)
class GraphicsEngine : public Singleton<GraphicsEngine> {
private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	std::string m_error = "";

	Vector_t<int> m_logical_size;

public:
	void init(ProgramSettings* settings);
	void deinit();

	void update();

	void draw_rect(Rect rect, bool filled = false, Color color = 0xFFFFFFFF);
	void draw_circle(Vector center, float radius, bool filled = false, Color color = 0xFFFFFFFF);
	void draw_line(Vector a, Vector b, Color color = 0xFFFFFFFF);
	void draw_point(Vector point, Color color = 0xFFFFFFFF);
	void draw_texture(Rect dest, Texture* tex);
	void draw_texture(Rect dest, Texture* tex, float angle, Vector center = {0,0});

	// TODO: Move this to another class?
	Texture* create_text(Font* font, const std::string& text, Color color = 0xFFFFFFFF);

	inline SDL_Window* get_window() { return m_window; }
	inline SDL_Renderer* get_renderer() { return m_renderer; }

	Color get_current_color();

	inline const std::string& get_error() { return m_error; }
	inline const Vector_t<int>& get_logical_size() { return m_logical_size; }
};

#endif // GRAPHICSENGINE_H
