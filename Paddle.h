#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Paddle
{
public:
	sf::RectangleShape shape;
	float speed = 5.0f; //5 пикселей за кадр

	Paddle(float width, float height);
	void update();
	void resize(float newWidth);
};


