#include "Game/Game.h"

/*COMMANDS:
	-CTRL + SHIFT + B TO BUILD AND RUN
	-COMMENT MULTIPLE LINES CTRL + K, CTRL + C
	-UNCOMMENT MULITPLE LINES CTRL + K, CTRL + U
*/

/*COMPLIER
	-TO NOT TREAT WARNINGS AS ERRORS REMOVE "-Werror"
	 IN THE "all.mk" FILE FROM "_CFLAGS_WARNINGS"
*/

int main()
{

#if defined(_DEBUG)
	std::cout << "DEBUGGING" << std::endl;

#endif
	Game* game = new Game();
	game->start();
	delete game;
	return 0;
}