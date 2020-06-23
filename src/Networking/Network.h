#ifndef NETWORK
#define NETWORK

#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>
#include <vector>
#include <map>

#include "TestPlayer.h"

class Network
{
private:

protected:
	//Globals
	sf::Mutex globalMutex;
	sf::IpAddress sendIp;
	unsigned short port;
	bool quit;
	float dt;
	sf::Clock gameClock;

	//UDP
	sf::UdpSocket UDP_Socket;

	//TCP
	sf::TcpSocket TCP_Socket;
public:
	Network();
	virtual ~Network();

	std::unordered_map<std::string, TestPlayer*> players;
	TestPlayer *p1;
	TestPlayer *p2;

	//UDP
	static void UDP_Traffic(Network* network);
	void UDP_Run();

	//TCP
	static void TCP_Traffic(Network* network);
	void  TCP_Run();
};


#endif