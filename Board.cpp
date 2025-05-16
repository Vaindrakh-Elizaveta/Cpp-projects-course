#include "Board.h"
#include "Game.h"
#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include<random>

const int BONUS_CHANCE = 50;
const int BONUS_RADIUS = 3;

Board::Board(int r, int c) : rows(r), cols(c), grid(r, vector<Cell>(c))
{
	rng.seed(static_cast<unsigned>(time(nullptr)));

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i][j].color = getRandomColor();
		}
	}
}

Color Board::getRandomColor()
{
	return static_cast<Color>(rng() % 5);
}

//проверка на допустимость обмена
bool Board::isValidSwap(int x1, int y1, int x2, int y2)
{
	int dx = abs(x1 - x2);
	int dy = abs(y1 - y2);

	return (dx + dy == 1);
}
//обмен клеток
void Board::swapCells(int row1, int col1, int row2, int col2)
{
	swap(grid[row1][col1], grid[row2][col2]);
}

bool Board::findMatches()
{
	bool found = false;
	//сбрасываем флаг уничтожени€
	for (auto& row : grid)
	{
		for (auto& cell : row)
		{
			cell.toBeDestroyed = false;
		}
	}

	//горизонтальные совпадени€
	for (int i = 0; i < rows; i++)
	{
		int count = 1;
		for (int j = 1; j < cols; j++)
		{
			if (grid[i][j].color != EMPTY && grid[i][j].color == grid[i][j - 1].color)
			{
				count++;
			}
			else
			{
				if (count >= 3)
				{
					found = true;
					for (int k = 0; k < count; k++)
					{
						grid[i][j - 1 - k].toBeDestroyed = true;
					}
				}
				count = 1;
			}
		}
		if (count >= 3)
		{
			found = true;
			for (int k = 0; k < count; k++)
			{
				grid[i][cols - 1 - k].toBeDestroyed = true;
			}
		}
	}

	//вертикальные совпадени€
	for (int j = 0; j < cols; j++)
	{
		int count = 1;
		for (int i = 1; i < rows; i++)
		{
			if (grid[i][j].color != EMPTY && grid[i][j].color == grid[i - 1][j].color)
			{
				count++;
			}
			else
			{
				if (count >= 3)
				{
					found = true;
					for (int k = 0; k < count; k++)
						grid[i - 1 - k][j].toBeDestroyed = true;
				}
				count = 1;
			}
		}
		if (count >= 3)
		{
			found = true;
			for (int k = 0; k < count; k++)
				grid[rows - 1 - k][j].toBeDestroyed = true;
		}
	}

	return found;
}

//удаление совпавших клеток
void Board::destroyMatches()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j].toBeDestroyed)
			{
				if (rng() % 100 < 25)
				{
					generateBonus(i, j);
				}
				grid[i][j].bonus = nullptr;   // —брасываем бонус (если есть)
				grid[i][j].bonusActivated = false;
				grid[i][j].alpha = 255;
				grid[i][j].fallOffset = { 0, 0 };
			}
		}
	}
}

void Board::updateAnimations(float deltaTime)
{
	//исчезновение клеток
	for (auto& row : grid)
	{
		for (auto& cell : row)
		{
			if (cell.toBeDestroyed && cell.alpha > 0)
			{
				cell.alpha -= 500 * deltaTime;
				if (cell.alpha <= 0)
				{
					cell.color = EMPTY;
					cell.toBeDestroyed = false;
					cell.alpha = 0;
				}
			}
		}
	}


	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (grid[i][j].fallOffset.y < 0)
			{
				grid[i][j].fallOffset.y += 1000 * deltaTime;
				if (grid[i][j].fallOffset.y >= 0)
					grid[i][j].fallOffset.y = 0;
			}
		}
	}
}

void Board::update(float deltaTime)
{
	std::cout << "STATE: ";
	switch (state) {
	case BoardState::Idle: std::cout << "Idle\n"; break;
	case BoardState::Fading: std::cout << "Fading\n"; break;
	case BoardState::Falling: std::cout << "Falling\n"; break;
	case BoardState::Filling: std::cout << "Filling\n"; break;
	case BoardState::BonusProcessing: std::cout << "BonusProcessing\n"; break;
	}

	updateAnimations(deltaTime);

	stateTimer += deltaTime;

	switch (state)
	{
	case BoardState::Idle: break;
	case BoardState::Fading:
	{
		bool allFaded = true;
		for (const auto& row : grid)
		{
			for (const auto& cell : row)
			{
				if (cell.toBeDestroyed && cell.alpha > 0)
				{
					allFaded = false;
					break;
				}
			}
			if (!allFaded) break;
		}

		if (allFaded)
		{
			applyGravity();
			state = BoardState::Falling;
			stateTimer = 0;
		}
		break;
	}
	case BoardState::Falling:
	{
		bool allSettled = true;
		for (const auto& row : grid)
		{
			for (const auto& cell : row)
			{
				if (cell.fallOffset.y < 0)
				{
					allSettled = false;
					break;
				}
			}
		}
		if (allSettled)
		{
			activatePendingBonuses();
			state = BoardState::BonusProcessing;
			stateTimer = 0;
		}
		break;
	}
	case BoardState::BonusProcessing:
		if (!isBonusActive())
		{
			bool hasDestroyed = false;
			for (const auto& row : grid)
			{
				for (const auto& cell : row)
				{
					if (cell.toBeDestroyed)
					{
						hasDestroyed = true;
						break;
					}
				}
				if (hasDestroyed) break;
			}
			if (hasDestroyed)
			{
				state = BoardState::Fading;
				stateTimer = 0;
			}
			else
			{
				fillEmptySpaces();
				state = BoardState::Filling;
				stateTimer = 0;
			}
		}
		break;
	case BoardState::Filling:
	{
		bool ready = true;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				const auto& cell = grid[i][j];

				if (cell.color != EMPTY && (cell.alpha < 255 || abs(cell.fallOffset.y) > 0.1f))
				{
					std::cout << "NOT READY at [" << i << "][" << j << "]: ";
					std::cout << "color=" << cell.color << " alpha=" << cell.alpha << " offset=" << cell.fallOffset.y << "\n";
					ready = false;
				}
			}
		}

		if (ready && stateTimer > 0.1f)
		{
			std::cout << "CHECKING FOR NEW MATCHES\n";
			if (findMatches())
			{
				std::cout << "NEW MATCH FOUND\n";
				destroyMatches();
				state = BoardState::Fading;
				stateTimer = 0;
			}
			else
			{
				std::cout << "NO MORE MATCHES\n";
				state = BoardState::Idle;
			}
		}
		break;
	}

	}
}

void Board::activatePendingBonuses()
{
	for (int i = 0;i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j].bonus && !grid[i][j].bonusActivated)
			{
				activateBonus(i, j);
			}
		}
	}
}

void Board::applyGravity()
{
	for (int col = 0; col < cols; col++)
	{
		for (int row = rows - 1; row >= 0; row--)
		{
			if (grid[row][col].color == EMPTY)
			{
				for (int k = row - 1; k >= 0; k--)
				{
					if (grid[k][col].color != EMPTY)
					{
						grid[row][col].color = grid[k][col].color;
						grid[row][col].alpha = 255;
						grid[row][col].fallOffset.y = (row - k) * -64;
						grid[row][col].toBeDestroyed = false;

						// ќчищаем источник
						grid[k][col].color = EMPTY;
						grid[k][col].alpha = 255;
						grid[k][col].fallOffset = { 0, 0 };
						grid[k][col].toBeDestroyed = false;

						break;
					}
				}
			}
		}
	}
}

//заполнение пустых мест новыми клетками
void Board::fillEmptySpaces()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j].color == EMPTY)
			{
				grid[i][j].color = getRandomColor();
				grid[i][j].fallOffset = { 0, 0 };
				grid[i][j].alpha = 255;
				grid[i][j].toBeDestroyed = false;
			}
		}
	}
}

//повторна€ проверка на совпадени€
void Board::processMatches()
{
	bool matchesFound = true;
	while (matchesFound)
	{
		matchesFound = findMatches();
		if (matchesFound)
		{
			destroyMatches();
			applyGravity();
			fillEmptySpaces();
		}
	}
}

void Board::handlePlayerMove(int x1, int y1, int x2, int y2)
{
	if (!isValidSwap(x1, y1, x2, y2)) return;

	swapCells(x1, y1, x2, y2);

	if (findMatches())
	{
		destroyMatches();
		state = BoardState::Fading;
		stateTimer = 0;
	}
	else
	{
		swapCells(x1, y1, x2, y2);
	}
}

void Board::print()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			char c = '.';
			switch (grid[i][j].color)
			{
			case RED: c = 'R'; break;
			case GREEN: c = 'G'; break;
			case YELLOW: c = 'Y'; break;
			case PURPLE: c = 'P'; break;
			case EMPTY: c = '.'; break;
			}
			cout << c << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
}

//бонусы
template<typename T>
T myMax(T a, T b)
{
	return (a > b) ? a : b;
}

template<typename T>
T myMin(T a, T b)
{
	return (a < b) ? a : b;
}

void Board::generateBonus(int sourceRow, int sourceCol)
{
	Color sourceColor = grid[sourceRow][sourceCol].color;
	
	int randOffset = rng() % (BONUS_RADIUS * 2 + 1) - BONUS_RADIUS;
	int targetRow = sourceRow + randOffset;
	targetRow = myMax(0, myMin(rows - 1, targetRow));

	randOffset = rng() % (BONUS_RADIUS * 2 + 1) - BONUS_RADIUS;
	int targetCol = sourceCol + randOffset;
	targetCol = myMax(0, myMin(cols - 1, targetCol));

	if (grid[targetRow][targetCol].color != EMPTY)
	{
		if (rng() % 2 == 0)
		{
			grid[targetRow][targetCol].bonus = std::make_shared<RecolorBonus>(sf::Vector2f(sourceCol, sourceRow));
		}
		else
		{
			grid[targetRow][targetCol].bonus = std::make_shared<BombBonus>(sf::Vector2f(sourceCol, sourceRow));
		}
	}
}

void Board::activateBonus(int row, int col)
{
	if (row < 0 || row >= rows || col < 0 || col >= cols) return;
	if (!grid[row][col].bonus) return;

	grid[row][col].bonusActivated = true;
	grid[row][col].bonus->apply(*this, row, col, rng);
	grid[row][col].bonus.reset();
	grid[row][col].bonusActivated = false;
}

bool Board::isBonusActive() const
{
	for (const auto& row : grid)
	{
		for (const auto& cell : row)
		{
			if (cell.bonus && !cell.bonusActivated) return true;
		}
	}

	return false;
}

bool Board::isCellEmpty(int row, int col) const 
{
	if (row < 0 || row >= rows || col < 0 || col >= cols) {
		return true;
	}
	return grid[row][col].color == EMPTY;
}

sf::Vector2f Board::getBonusSource(int row, int col) const 
{
	if (row < 0 || row >= rows || col < 0 || col >= cols) 
	{
		return { -1, -1 };
	}
	return grid[row][col].bonusSource;
}

void Board::setCellColor(int row, int col, Color color) 
{
	if (row >= 0 && row < rows && col >= 0 && col < cols) 
	{
		grid[row][col].color = color;
	}
}

void RecolorBonus::apply(Board& board, int row, int col, std::mt19937& rng)
{
	if (row < 0 || row >= board.getRows() || col < 0 || col >= board.getCols()) return;
	if (board.isCellEmpty(row, col)) return;

	sf::Vector2f source = board.getBonusSource(row, col);
	if (source.x == -1 || source.y == -1) return;
	
	Color targetColor = board.getCellColor(source.y, source.x);
	int recolored = 0;
	int maxAttempts = 100;

	board.setCellColor(row, col, targetColor);
	recolored++;

	while (recolored < 3 && maxAttempts > 0)
	{
		int r = source.y + (rng() % (BONUS_RADIUS * 2 + 1) - BONUS_RADIUS);
		int c = source.x + (rng() % (BONUS_RADIUS * 2 + 1) - BONUS_RADIUS);

		if (r >= 0 && r < board.getRows() && c < board.getCols() && c >= 0 &&
			!(r == source.y && c == source.x) &&
			(abs(r - source.y) + abs(c - source.x) > 1) &&
			board.getCellColor(r, c) != EMPTY)
		{
			board.setCellColor(r, c, targetColor);
			recolored++;
		}
		maxAttempts--;
	}
}

void Board::markCellForDestruction(int row, int col) 
{
	if (row >= 0 && row < rows && col >= 0 && col < cols) 
	{
		grid[row][col].toBeDestroyed = true;
		grid[row][col].alpha = 255;
	}
}

bool Board::isCellMarkedForDestruction(int row, int col) const 
{
	if (row < 0 || row >= rows || col < 0 || col >= cols) 
	{
		return false;
	}
	return grid[row][col].toBeDestroyed;
}

void BombBonus::apply(Board& board, int row, int col, std::mt19937& rng)
{
	int destroyed = 0;

	// ”ничтожаем клетку с бомбой
	if (board.getCellColor(row, col) != EMPTY) 
	{
		board.markCellForDestruction(row, col);
		destroyed++;
	}

	while (destroyed < 5)
	{
		int r = rng() % board.getRows();
		int c = rng() % board.getCols();

		if (!board.isCellEmpty(r, c) && !board.isCellMarkedForDestruction(r, c))
		{
			board.markCellForDestruction(r, c);
			destroyed++;
		}
	}

}

sf::Color Board::getBonusColor(int row, int col) const 
{
	if (row < 0 || row >= rows || col < 0 || col >= cols) return sf::Color::Transparent;
	if (!grid[row][col].bonus) return sf::Color::Transparent;
	return grid[row][col].bonus->getIndicatorColor();
}

