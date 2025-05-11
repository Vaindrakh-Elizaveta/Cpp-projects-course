#pragma once
#include"Cell.h"
#include<iostream>
#include<vector>
#include<random>
#include<ctime>

using namespace std;

enum class BoardState{ Idle, Fading, Falling, BonusProcessing, Filling };

class Board
{
private:
	vector<vector<Cell>> grid;
	int rows, cols;
	std::mt19937 rng;

	Color getRandomColor();

	BoardState state = BoardState::Idle;
	float stateTimer = 0.f;

public:
	Board(int rows, int cols);

	int getRows() const { return rows; }
	int getCols() const { return cols; }
	Color getCellColor(int row, int col) const { return grid[row][col].color; }
	sf::Vector2f getFallOffset(int row, int col) const { return grid[row][col].fallOffset; }
	int getAlpha(int row, int col) const { return grid[row][col].alpha; }

	bool isValidSwap(int x1, int y1, int x2, int y2);
	void swapCells(int row1, int col1, int row2, int col2);
	bool findMatches();
	void destroyMatches();
	void applyGravity();
	void fillEmptySpaces();
	void processMatches();
	void handlePlayerMove(int x1, int y1, int x2, int y2);
	void updateAnimations(float deltaTime);
	void update(float deltaTime);
	//бонусы
	void generateBonus(int sourceRow, int sourceCol);
	void applyRecolorBonus(int row, int col);
	void applyBombBonus(int row, int col);
	void activateBonus(int row, int col);

	bool hasBonus(int row, int col) const { return grid[row][col].bonus != NONE; }
	BonusType getBonusType(int row, int col) const { return grid[row][col].bonus; }
	bool isBonusActive() const;
	void activatePendingBonuses();

	void print();
	bool isAnimating() const { return state != BoardState::Idle; }
};

