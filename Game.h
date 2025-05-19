#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include <memory>
#include"StandartBlock.h"
#include "UnbreakableBlock.h"
#include "SpeedUpBlock.h"
#include"BonusBlock.h"
#include"Bonus.h"

class Game
{
public:
	Game();
	void run();

private:
	void processEvents();
	void update();
	void render();
	void generateBlocks();
	void handleBallMiss();
	void showGameOverScreen();

	sf::RenderWindow window;
	Paddle paddle;
	Ball ball;
	std::vector<std::unique_ptr<Block>> blocks;

	int score;
	sf::Font font;
	sf::Text scoreText;

	int lives = 4; //количество жизней
	int defeats = 0; //счётчик поражений
	sf::Text livesText; //Текст для отображения жизней

	sf::Clock blinkTimer;  // Таймер для анимации
	bool isBlinking = false; // Флаг мигания
	float blinkDuration = 0.5f; // Длительность мигания

	
	std::vector<std::unique_ptr<FallingBonus>> activeBonuses;

	bool oneTimeBottomActive = false;
	
	bool isRunning;
	bool gameOver = false;
};

