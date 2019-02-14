#include "Game.h"

int main()
{
	Game game("LALA", 640, 480, 3, 3, false);
	while (!game.getWindowShouldClose())
    {
		game.update();
		game.render();
		}

    return 0;
}




