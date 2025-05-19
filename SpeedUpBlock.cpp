#include"SpeedUpBlock.h"
#include"Ball.h"

SpeedUpBlock::SpeedUpBlock(float x, float y, float width, float height)
	: Block(x, y, width, height, sf::Color::Red) {}

void SpeedUpBlock::onHit(Ball& ball, int& playerScore)
{
	ball.velocity *= 1.2f;
	ball.velocity.y = -ball.velocity.y;
	isDestroyed = true;
	playerScore += 2;
}