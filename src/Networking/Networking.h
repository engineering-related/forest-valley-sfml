#include <SFML/Network.hpp>
#include <iostream>
#include "TCP.h"
#include "UDP.h"

class Networking
{
private:
	/* data */

	unsigned short cliPort;
	unsigned short serPort;
	bool isServer; // am I the server ?

public:
	Networking(/* args */);
	~Networking();
	void networkInit();
	unsigned short getCliPort();
	unsigned short getSerPort();

};

