#ifndef BALL_H
#define BALL_H

#include <actor.hpp>

class Ball : public Actor
{
public:
    Ball(Vector pos = {0, 0}, Vector vel = {2, 2});

    void update() override;
    void draw() override;

private:
    int m_col = 0xFFFFFFFF;

    bool check_horizontal_bound();
    bool check_vertical_bound();
};

#endif // BALL_H
