#pragma once
#include <SFML/Graphics.hpp>
#include"Block.h"
#include"Paddle.h"

class Ball
{
public:
	sf::CircleShape shape;
	sf::Vector2f velocity{ -3.0f, -3.0f };//скорость шарика в горизонтальном и вертикальном направлениях

	bool stuckToPaddle = false;

	Ball(float radius);
	void update(Paddle& paddle);
	bool checkCollision(const sf::RectangleShape& rect);
};


