#ifndef BALL_H
#define BALL_H

#include <actor.hpp>

class Ball : public Actor
{
	int m_col = 0xFFFFFFFF;
public:
	Ball(Vector pos, Vector vel);
	Ball();

	void update() override;
	void draw() override;

	bool check_horizontal_bound();
	bool check_vertical_bound();
};

#endif // BALL_H
