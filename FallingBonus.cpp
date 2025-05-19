#include"FallingBonus.h"

FallingBonus::FallingBonus(float x, float y, std::unique_ptr<Bonus> bonus, sf::Color color)
	: bonus(std::move(bonus))
{
	shape.setSize({ 20, 20 });
	shape.setFillColor(color);
	shape.setPosition(x, y);
}

void FallingBonus::update()
{
	shape.move(0, 2.5f);
}

void FallingBonus::draw(sf::RenderWindow& window)
{
	window.draw(shape);
}

bool FallingBonus::checkCollision(Paddle& paddle, Ball& ball)
{
	if (shape.getGlobalBounds().intersects(paddle.shape.getGlobalBounds()))
	{
		bonus->apply(paddle, ball);
		isCollected = true;
		return true;
	}

	return false;
}


