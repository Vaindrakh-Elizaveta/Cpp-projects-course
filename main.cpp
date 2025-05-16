#include "Board.h"
#include<locale.h>
#include <windows.h>
#include"Game.h"

int main()
{
	Game game(8, 8);
	game.run();

	return 0;
}


