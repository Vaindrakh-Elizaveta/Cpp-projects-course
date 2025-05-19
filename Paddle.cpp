#include"Paddle.h"

Paddle::Paddle(float width, float height)
{
	shape.setSize(sf::Vector2f(width, height));
	shape.setFillColor(sf::Color::White);
	shape.setPosition(400 - width / 2, 550);
}

void Paddle::update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		shape.move(-speed, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		shape.move(speed, 0);
	}

	if (shape.getPosition().x < 0) shape.setPosition(0, 550);
	if (shape.getPosition().x + shape.getSize().x > 800)
	{
		shape.setPosition(800 - shape.getSize().x, 550);
	}
}

void Paddle::resize(float newWidth)
{
	sf::Vector2f oldSize = shape.getSize();
	
	float oldWidth = oldSize.x;
	shape.setSize(sf::Vector2f(newWidth, oldSize.y));

	//центрировать каретку, чтобы она не скакала
	sf::Vector2f pos = shape.getPosition();
	float shift = (newWidth - oldWidth) / 2.0f;
    shape.setPosition(pos.x - shift, pos.y);
}

