#include"UnbreakableBlock.h"
#include"Ball.h"

UnbreakableBlock::UnbreakableBlock(float x, float y, float width, float height)
	: Block(x, y, width, height, sf::Color::Blue) {}

void UnbreakableBlock::onHit(Ball& ball, int& playerScore)
{
	ball.velocity.y = -ball.velocity.y;
}