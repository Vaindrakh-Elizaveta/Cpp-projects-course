#pragma once
#include"Block.h"

class SpeedUpBlock : public Block
{
public:
	SpeedUpBlock(float x, float y, float width, float height);
	void onHit(Ball& ball, int& playerScore) override;
};