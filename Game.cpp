#include"Game.h"

bool globalOneTimeBottomFlag = false;

Game::Game() :
	window(sf::VideoMode(800, 600), "Arkanoid"),
	paddle(100, 20),
	ball(10),
	score(0),
	isRunning(true)
{
	window.setFramerateLimit(60);

	//»нициализаци€ текста
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cerr << "Error loading font!" << std::endl;
		isRunning = false;
	}

	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10, 10);
	scoreText.setString("Score: 0");

	//настройка текста жизней
	livesText.setFont(font);
	livesText.setCharacterSize(24);
	livesText.setFillColor(sf::Color::White);
	livesText.setPosition(700, 10);
	scoreText.setString("Lives: " + std::to_string(lives));

	generateBlocks();
}

void Game::generateBlocks()
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 10; ++col)
		{
			float x = col * 80 + 10;
			float y = row * 30 + 50;
			if (row == 0)
				blocks.push_back(std::make_unique<UnbreakableBlock>(x, y, 70, 20));
			else if (row == 1)
				blocks.push_back(std::make_unique<SpeedUpBlock>(x, y, 70, 20));
			else if (row == 2)
				blocks.push_back(std::make_unique<BonusBlock>(x, y, 70, 20));
			else
				blocks.push_back(std::make_unique<StandartBlock>(x, y, 70, 20, sf::Color::Green, 2));
		}
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
			isRunning = false;
		}
	}
}

void Game::handleBallMiss()
{
	if (gameOver) return;

	lives--;
	isBlinking = true;
	blinkTimer.restart();
	defeats++;
	score = std::max(0, score - 10);

	//обновл€ем тексты
	scoreText.setString("Score: " + std::to_string(score));
	livesText.setString("Lives: " + std::to_string(std::max(0, lives)));
	//уменьшаем каретку
	float newWidth = std::max(50.0f, paddle.shape.getSize().x - 20.0f);
	paddle.resize(newWidth);
	//сбрасываем позицию м€ча
	ball.shape.setPosition(400, 300);
	ball.velocity = { -3.0f, -3.0f };

	//ѕроверка окончани€ игры
	if (lives <= 0)
	{
		gameOver = true;
		std::cerr << "Game Over! Final Score: " << score
			<< ", Defeats: " << defeats << std::endl;
		isRunning = false;
	}
}

void Game::update()
{
	if (gameOver) return;
	//ќбновление игрового состо€ни€
	paddle.update();
	ball.update(paddle);

	//ѕроверка вылета м€ча за низ экрана
	if (ball.shape.getPosition().y + 2 * ball.shape.getRadius() > 600) 
	{
		if (oneTimeBottomActive) 
		{
			oneTimeBottomActive = false;
			ball.velocity.y *= -1;
			ball.shape.setPosition(ball.shape.getPosition().x, 600 - 2 * ball.shape.getRadius() - 1);
		}
		else 
		{
			handleBallMiss();
		}
	}

	if (isBlinking)
	{
		float elapsed = blinkTimer.getElapsedTime().asSeconds();
		if (elapsed < blinkDuration)
		{
			int blinkPhase = static_cast<int>(elapsed / 0.1f) % 2;
			paddle.shape.setFillColor(blinkPhase ? sf::Color::Red : sf::Color::White);
		}
		else
		{
			isBlinking = false;
			paddle.shape.setFillColor(sf::Color::White);
		}
	}

	//ѕроверка столкновений шарика с блоками
	for (auto& block : blocks)
	{
		if (!block->isDestroyed && ball.checkCollision(block->shape))
		{
			block->onHit(ball, score);
			if (auto* bonusBlock = dynamic_cast<BonusBlock*>(block.get())) {
				bonusBlock->spawnBonus(activeBonuses);
			}
			scoreText.setString("Score: " + std::to_string(score));
			break;
		}
	}


	for (auto it = activeBonuses.begin(); it != activeBonuses.end(); ) 
	{
		(*it)->update();
		if ((*it)->checkCollision(paddle, ball)) 
		{
			it = activeBonuses.erase(it);
		}
		else 
		{
			++it;
		}
	}


	if (ball.checkCollision(paddle.shape))
	{
		if (!ball.stuckToPaddle)
		{
			ball.velocity.y = -abs(ball.velocity.y);
		}
	}

	if (globalOneTimeBottomFlag) 
	{
		oneTimeBottomActive = true;
		globalOneTimeBottomFlag = false;
	}


	//проверка завершени€ уровн€
	isRunning = !std::all_of(blocks.begin(), blocks.end(),
		[](const std::unique_ptr<Block>& b) { return b->isDestroyed; });
}

void Game::render()
{
	//отрисовка
	window.clear(sf::Color::Black);
	window.draw(paddle.shape);
	window.draw(ball.shape);

	//рисуем только "живые блоки"
	for (auto& block : blocks)
	{
		if (!block->isDestroyed)
		{
			window.draw(block->shape);
		}
	}
	//отрисовка падающих бонусов
	for (auto& bonus : activeBonuses)
	{
		bonus->draw(window);
	}

	window.draw(scoreText);
	window.draw(livesText);
	window.display();
}

void::Game::showGameOverScreen()
{
	sf::Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setString("GAME OVER\nFinal Score: " + std::to_string(score) +
		"\nPress ESC to exit");

	gameOverText.setCharacterSize(50);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setPosition(200, 250);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed ||
				(event.type == sf::Event::KeyPressed &&
					event.key.code == sf::Keyboard::Escape))
			{
				window.close();
				return;
			}
		}
		window.clear();
		window.draw(gameOverText);
		window.display();
	}
}

void Game::run()
{
	while (window.isOpen() && !gameOver && isRunning)
	{
		processEvents();
		update();
		render();
	}

	if (gameOver)
	{
		showGameOverScreen();
	}

	if (std::all_of(blocks.begin(), blocks.end(),
		[](const std::unique_ptr<Block>& b) { return b->isDestroyed; }))
	{
		std::cout << "Level Complete! Score: " << score << std::endl;
	}
}



