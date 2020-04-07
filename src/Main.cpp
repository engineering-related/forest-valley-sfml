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

//Some animals?
//https://opengameart.org/content/lpc-wolf-animation
//https://opengameart.org/content/lpc-style-farm-animals
//https://opengameart.org/content/lpc-birds
//https://opengameart.org/content/bunny-rabbit-lpc-style-for-pixelfarm
//https://opengameart.org/content/lpc-goat
//https://opengameart.org/content/deer


int main()
{
	Game* game = new Game();
	game->start();
	delete game;
	return 0;
}