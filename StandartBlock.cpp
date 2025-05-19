#include"StandartBlock.h"
#include"Ball.h"

StandartBlock::StandartBlock(float x, float y, float width, float height, sf::Color color, int health)
	: Block(x, y, width, height, color), health(health)
{}

void StandartBlock::onHit(Ball& ball, int& playerScore)
{
	if (isDestroyed) return;

	health--;
	playerScore++;
	if (health <= 0)
	{
		isDestroyed = true;
	}

	ball.velocity.y = -ball.velocity.y;
}

