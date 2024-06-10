#include "ball.hpp"

#include <iostream>

#include <painter.hpp>

constexpr float RADIUS = 50.f;

Ball::Ball():
	Actor()
{
	m_bbox = {0, 0, RADIUS*2, RADIUS*2};
	m_vel = {2, 2};
}

void Ball::update()
{
	bool hor = check_horizontal_bound();
	bool ver = check_vertical_bound();

	if (hor) {
		m_vel.x = -m_vel.x;
	}

	if (ver) {
		m_vel.y = -m_vel.y;
	}

	if (hor && ver)
	{
		std::cout << "CORNER!!!!" << std::endl;
	}

	Actor::update();
}

void Ball::draw()
{
	Painter::get().get().draw_circle({m_bbox.x + RADIUS, m_bbox.y + RADIUS}, RADIUS, true);
}

bool Ball::check_horizontal_bound()
{
	int w = -1;
	SDL_GetWindowSize(Painter::get().get_window(), &w, nullptr);

	return m_bbox.x < 0.f || m_bbox.x + m_bbox.w > static_cast<float>(w);
}

bool Ball::check_vertical_bound()
{
	int h = -1;
	SDL_GetWindowSize(Painter::get().get_window(), nullptr, &h);

	return m_bbox.y < 0.f || m_bbox.y + m_bbox.h > static_cast<float>(h);
}
