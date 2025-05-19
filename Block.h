#pragma once

#include <SFML/Graphics.hpp>

class Ball;

class Block
{
public:
	sf::RectangleShape shape;
	bool isDestroyed = false;

	Block(float x, float y, float width, float height, sf::Color color);
	virtual ~Block() = default;

	virtual void onHit(Ball& ball, int& playerScore) = 0;
	
	virtual void update() {}
};

