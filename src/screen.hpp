#ifndef SCREEN_HPP
#define SCREEN_HPP

class Screen
{
public:
	Screen();

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
};

#endif // SCREEN_HPP
