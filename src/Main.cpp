#include "Game/Game.h"

/*COMMANDS:
	-CTRL + SHIFT + B TO BUILD AND RUN
	-COMMENT MULTIPLE LINES CTRL + K, CTRL + C
	-UNCOMMENT MULITPLE LINES CTRL + K, CTRL + U
*/

int main()
{

#if defined(_DEBUG)
	std::cout << "DEBUGGING" << std::endl;

#endif
	Game *game = new Game();
	game->start();

	return 0;
}