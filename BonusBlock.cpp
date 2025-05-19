#include"BonusBlock.h"
#include<random>

BonusBlock::BonusBlock(float x, float y, float w, float h)
    : Block(x, y, w, h, sf::Color::Yellow) {
    bonus = std::make_unique<EnlargePaddleBonus>();
}

void BonusBlock::onHit(Ball& ball, int& score) 
{
    score += 2;
    ball.velocity.y = -ball.velocity.y;
    isDestroyed = true;
}


std::random_device rd;                            // Источник энтропии
std::mt19937 gen(rd());                           // Генератор псевдослучайных чисел
std::uniform_int_distribution<> distrib(0, 4);

void BonusBlock::spawnBonus(std::vector<std::unique_ptr<FallingBonus>>& activeBonuses) 
{
    int bonusType = distrib(gen);;
    std::unique_ptr<Bonus> bonus;
    sf::Color color;

    switch (bonusType)
    {
    case 0:
        bonus = std::make_unique<EnlargePaddleBonus>();
        color = sf::Color::Yellow;
        break;
    case 1:
        bonus = std::make_unique<RandomTrajectoryBonus>();
        color = sf::Color::Red;
        break;
    case 2:
        bonus = std::make_unique<SpeedChangeBonus>();
        color = sf::Color::Blue;
        break;
    case 3:
        bonus = std::make_unique<StickyBallBonus>();
        color = sf::Color::Magenta;
        break;
    case 4:
        bonus = std::make_unique<OneTimeBottomBonus>();
        color = sf::Color::Green;
        break;
    default:
        bonus = std::make_unique<EnlargePaddleBonus>();
        color = sf::Color::Yellow;
        break;
    }

    activeBonuses.push_back(std::make_unique<FallingBonus>(
        shape.getPosition().x,
        shape.getPosition().y,
        std::move(bonus),
        color
        ));
}

