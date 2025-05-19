#pragma once

#include <SFML/Graphics.hpp>
#include"Paddle.h"
#include"Ball.h"

class Bonus
{
public:
	virtual ~Bonus() = default;
	virtual void apply(Paddle& paddle, Ball& ball) = 0;
};

class RandomTrajectoryBonus : public Bonus
{
public: 
	void apply(Paddle& paddle, Ball& ball) override;
};

class EnlargePaddleBonus : public Bonus
{
public:
	void apply(Paddle& paddle, Ball& ball) override;
};

class SpeedChangeBonus : public Bonus
{
public:
	void apply(Paddle& paddle, Ball& ball) override;
};

class StickyBallBonus : public Bonus 
{
public:
	void apply(Paddle& paddle, Ball& ball) override;
};

class OneTimeBottomBonus : public Bonus
{
public:
	void apply(Paddle& paddle, Ball& ball) override;
};


