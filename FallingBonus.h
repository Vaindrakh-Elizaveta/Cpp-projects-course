#pragma once
#include"Bonus.h"
#include"SFML/Graphics.hpp"

class FallingBonus
{
public:
	FallingBonus(float x, float y, std::unique_ptr<Bonus> bonus, sf::Color color);

	void update();
	void draw(sf::RenderWindow& window);
	bool checkCollision(Paddle& paddle, Ball& ball);

	bool isCollected = false;

private:
	sf::RectangleShape shape;
	std::unique_ptr<Bonus> bonus;
};


