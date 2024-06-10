#ifndef BALL_H
#define BALL_H

#include <types.hpp>

// TODO: Make Actor class and inherit from it
class Ball
{
public:
	Ball();

	void update();
	void draw();

	bool check_horizontal_bound();
	bool check_vertical_bound();

private:
	Vector m_pos;
	Vector m_vel;
};

#endif // BALL_H
