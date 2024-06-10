#include "ball.hpp"

#include <iostream>

#include <painter.hpp>

constexpr float RADIUS = 50.f;

Ball::Ball():
	m_pos{0, 0},
	m_vel{2, 2}
{

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

	m_pos.x += m_vel.x;
	m_pos.y += m_vel.y;
}

void Ball::draw()
{
	Painter::get().get().draw_circle({m_pos.x + RADIUS, m_pos.y + RADIUS}, RADIUS, true);
}

bool Ball::check_horizontal_bound()
{
	int w = -1;
	SDL_GetWindowSize(Painter::get().get_window(), &w, nullptr);

	return m_pos.x < 0.f || m_pos.x + (RADIUS*2) > static_cast<float>(w);
}

bool Ball::check_vertical_bound()
{
	int h = -1;
	SDL_GetWindowSize(Painter::get().get_window(), nullptr, &h);

	return m_pos.y < 0.f || m_pos.y + (RADIUS*2) > static_cast<float>(h);
}
