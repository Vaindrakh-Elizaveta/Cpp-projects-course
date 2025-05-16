#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class Board;

class Bonus {
protected: 
	sf::Vector2f sourcePosition;

public: 
	Bonus(const sf::Vector2f& source) : sourcePosition(source) {}
	virtual ~Bonus() = default;

	virtual void apply(Board& board, int row, int col, std::mt19937& rng) = 0;
	virtual sf::Color getIndicatorColor() const = 0;
};

class RecolorBonus : public Bonus
{
public: 
	RecolorBonus(const sf::Vector2f& source) : Bonus(source) {}

	void apply(Board& board, int row, int col, std::mt19937& rng) override;
	sf::Color getIndicatorColor() const override { return sf::Color(0, 255, 255); }
};

class BombBonus : public Bonus
{
public:
	BombBonus(const sf::Vector2f& source) : Bonus(source) {}

	void apply(Board& board, int row, int col, std::mt19937& rng) override;
	sf::Color getIndicatorColor() const override { return sf::Color(0, 0, 0); }
};


