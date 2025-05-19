#include"Block.h"
#include"Ball.h"


Block::Block(float x, float y, float width, float height, sf::Color color)
{
	shape.setSize(sf::Vector2f(width, height));
	shape.setPosition(x, y);
	shape.setFillColor(color);
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color::White);
}







