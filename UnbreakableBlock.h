#pragma once
#include"Block.h"

class UnbreakableBlock : public Block
{
public:
	UnbreakableBlock(float x, float y, float width, float height);
	void onHit(Ball& ball, int& playerScore) override;
};