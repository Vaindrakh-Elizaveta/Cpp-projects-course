#pragma once
#include <SFML/Graphics.hpp>
#include"Board.h"



class Game
{
private:
	sf::RenderWindow window;
	Board board;

	static constexpr int cellSize = 64;

	bool isFirstClick = true;
	sf::Vector2i firstSelectedCell;
	sf::Vector2i secondSelectedCell;

public:
	Game(int rows, int cols);
	void run();

private:
	void processEvents();
	void update(float deltaTime);
	void render();
	void handleMouseClick(int x, int y);
	void SwapCells();
	void drawSelection();
	void drawBonus(int row, int col, const sf::Vector2f& pos);
};

