#include"Bonus.h"
#include <cstdlib>
#include<cmath>
#include<random>
#include<iostream>
#include"Game.h"

extern bool globalOneTimeBottomFlag;

//изменение траектории шарика
void RandomTrajectoryBonus::apply(Paddle& paddle, Ball& ball)
{
	float angle = static_cast<float>((rand() % 120 + 30) * 3.1415 / 180.0);
	float speed = std::sqrt(ball.velocity.x * ball.velocity.x + ball.velocity.y * ball.velocity.y);
	ball.velocity.x = speed * std::cos(angle);
	ball.velocity.y = -std::abs(speed * std::sin(angle));
}

//Изменение размера каретки
void EnlargePaddleBonus::apply(Paddle& paddle, Ball& ball)
{
	float scaleFactor = (std::rand() % 2 == 0) ? 1.5f : 0.8f;
	sf::Vector2f size = paddle.shape.getSize();
	float oldWidth = size.x;
	size.x *= scaleFactor;
	paddle.shape.setSize(size);

	//центрировать каретку, чтобы она не скакала
	sf::Vector2f pos = paddle.shape.getPosition();
	float shift = (size.x - oldWidth) / 2.0f;
	paddle.shape.setPosition(pos.x - shift, pos.y);
}

//Изменение скорости шарика
void SpeedChangeBonus::apply(Paddle& paddle, Ball& ball)
{
	float speed = std::sqrt(ball.velocity.x * ball.velocity.x + ball.velocity.y * ball.velocity.y);

	int randType = rand() % 2;
	float value;
	switch (randType)
	{
	case 0: 
		value = 1.2f;
		break;
	case 1:
		value = 0.8f;
		break;
	default:
		value = 1.2f;
		break;
	}

	float newSpeed = speed * value;
	float angle = std::atan2(ball.velocity.y, ball.velocity.x);
	ball.velocity.x = newSpeed * std::cos(angle);
	ball.velocity.y = newSpeed * std::sin(angle);
}


//прилипание мячика к каретке
void StickyBallBonus::apply(Paddle& paddle, Ball& ball)
{
	ball.stuckToPaddle = true;
}

//одноразовое дно
void OneTimeBottomBonus::apply(Paddle& paddle, Ball& ball)
{
	globalOneTimeBottomFlag = true;
}
