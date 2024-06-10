#ifndef BALL_H
#define BALL_H

#include <actor.hpp>

class Ball : public Actor
{
public:
	Ball();

	void update() override;
	void draw() override;

	bool check_horizontal_bound();
	bool check_vertical_bound();
};

#endif // BALL_H
