#include "Game/Game.h"
#include "Networking/Networking.hpp"

/*COMMANDS:
	-CTRL + SHIFT + B TO BUILD AND RUN
	-COMMENT MULTIPLE LINES CTRL + K, CTRL + C
	-UNCOMMENT MULITPLE LINES CTRL + K, CTRL + U
*/

/*COMPLIER
	-TO NOT TREAT WARNINGS AS ERRORS REMOVE "-Werror"
	 IN THE "all.mk" FILE FROM "_CFLAGS_WARNINGS"

	-TO UPPRESS WARNING "unused variable" REMOVE
	"-Wunused"IN THE "all.mk" FILE FROM "_CFLAGS_WARNINGS"
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
	//Game* game = new Game();
	//game->start();

	//Networking class testing
	char connectionType;

	std::cout << "s)erver or c)lient" << std::endl;
	std::cin >> connectionType;

	if (connectionType == 's')
	{
		UDP_Server server;
		server.init();
	}
	else
	{
		UDP_Client client();
		client.connectToServer();
	}

}
