#include <SFML/Graphics.hpp>
#include <iostream>
#include"Ball.h"
#include"Paddle.h"
#include<vector>
#include"Block.h"
#include"Game.h"

int main()
{
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	Game game;
	game.run();

	return 0;
}
