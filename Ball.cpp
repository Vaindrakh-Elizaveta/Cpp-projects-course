#include"Ball.h"
#include"Block.h"


Ball::Ball(float radius)
{
	shape.setRadius(radius);
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(400, 300); //Центр экрана
}

void Ball::update(Paddle& paddle)
{
	shape.move(velocity);

	//Отскок от стен
	if (shape.getPosition().x < 0 || shape.getPosition().x + 2 * shape.getRadius() > 800)
	{
		velocity.x = -velocity.x;
	}
	if (shape.getPosition().y < 0)
	{
		velocity.y = -velocity.y;
	}

	if (stuckToPaddle)
	{
		sf::Vector2f paddlePos = paddle.shape.getPosition();
		float paddleWidth = paddle.shape.getSize().x;
		float ballRadius = shape.getRadius();
		shape.setPosition(paddlePos.x + paddleWidth / 2 - ballRadius, paddlePos.y - 2 * ballRadius);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			stuckToPaddle = false;
			velocity = { 3.f, -4.f };
		}
	}
}

bool Ball::checkCollision(const sf::RectangleShape& rect)
{
	return shape.getGlobalBounds().intersects(rect.getGlobalBounds());
}
