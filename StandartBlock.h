#pragma once
#include"Block.h"

class StandartBlock : public Block
{
private:
	int health;
public:
	StandartBlock(float x, float y, float width, float height, sf::Color color, int health);
	void onHit(Ball& ball, int& playerScore) override;
};