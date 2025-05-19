#pragma once
#include"Block.h"
#include"Bonus.h"
#include"FallingBonus.h"
#include<memory>

class BonusBlock : public Block
{
public:
	BonusBlock(float x, float y, float w, float h);

	void onHit(Ball& ball, int& playerScore) override;
	void spawnBonus(std::vector<std::unique_ptr<FallingBonus>>& activeBonuses);

private:
	std::unique_ptr<Bonus> bonus;
	bool used = false;
};

