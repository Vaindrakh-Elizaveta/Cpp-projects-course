#include "Board.h"
#include "Game.h"
#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <algorithm>

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
	//сбрасываем флаг уничтожения
	for (auto& row : grid)
	{
		for (auto& cell : row)
		{
			cell.toBeDestroyed = false;
		}
	}

	//горизонтальные совпадения
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

	//вертикальные совпадения
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
				grid[i][j].bonus = NONE;   // Сбрасываем бонус (если есть)
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
			if (grid[i][j].bonus != NONE && !grid[i][j].bonusActivated)
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

						// Очищаем источник
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

//повторная проверка на совпадения
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
		grid[targetRow][targetCol].bonus = (rng() % 2 == 0) ? RECOLOR : BOMB;
		grid[targetRow][targetCol].bonusSource = sf::Vector2f(sourceRow, sourceCol);
	}
}

void Board::activateBonus(int row, int col)
{
	if (row < 0 || row >= rows || col < 0 || col >= cols) return;

	grid[row][col].bonusActivated = true;

	switch (grid[row][col].bonus)
	{
	case RECOLOR:
		applyRecolorBonus(row, col);
		break;
	case BOMB:
		applyBombBonus(row, col);
		state = BoardState::Fading;
		stateTimer = 0;
		break;
	case NONE:
		return;
	}

	grid[row][col].bonus = NONE;
	grid[row][col].bonusActivated = false;
	grid[row][col].bonusSource = sf::Vector2f(-1, -1);
}

void Board::applyRecolorBonus(int row, int col)
{
	if (row < 0 || row >= rows || col < 0 || col >= cols) return;
	if (grid[row][col].color == EMPTY) return;

	int sourceRow = grid[row][col].bonusSource.y;
	int sourceCol = grid[row][col].bonusSource.x;
	if (sourceRow == -1 || sourceCol == -1) return;

	Color targetColor = grid[sourceRow][sourceCol].color;
	int recolored = 0;
	int maxAttempts = 100;
	
	grid[row][col].color = targetColor;
	recolored++;

	while (recolored < 3 && maxAttempts > 0)
	{
		int r = sourceRow + (rng() % (BONUS_RADIUS * 2 + 1) - BONUS_RADIUS);
		int c = sourceCol + (rng() % (BONUS_RADIUS * 2 + 1) - BONUS_RADIUS);

		if (r >= 0 && r < rows && c < cols && c >= 0 &&
			!(r == sourceRow && c == sourceCol) &&
			(abs(r - sourceRow) + abs(c - sourceCol) > 1) &&
			grid[r][c].color != EMPTY)
		{
			grid[r][c].color = targetColor;
			recolored++;
		}
		maxAttempts--;
	}

}

void Board::applyBombBonus(int row, int col)
{
	int destroyed = 0;
	
	// Уничтожаем клетку с бомбой
	if (grid[row][col].color != EMPTY) {
		grid[row][col].toBeDestroyed = true;
		destroyed++;
	}

	while (destroyed < 5)
	{
		int r = rng() % rows;
		int c = rng() % cols;

		if (grid[r][c].color != EMPTY && !grid[r][c].toBeDestroyed)
		{
			grid[r][c].toBeDestroyed = true;  // Помечаем для анимации
			grid[r][c].alpha = 255;
			destroyed++;
		}
	}

}

bool Board::isBonusActive() const
{
	for (const auto& row : grid)
	{
		for (const auto& cell : row)
		{
			if (cell.bonus != NONE && !cell.bonusActivated) return true;
		}
	}

	return false;
}


