#ifndef NETWORK
#define NETWORK

#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>
#include <vector>
#include <map>

class Network
{
private:

protected:
	sf::Mutex globalMutex;
	sf::IpAddress sendIp;
	unsigned short port;
	bool quit;
	float dt;
	sf::Clock gameClock;
public:

	Network();
	virtual ~Network();


};



#endif