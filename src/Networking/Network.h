#ifndef NETWORK
#define NETWORK

#include <SFML/Network.hpp>
#include <iostream>
#include <string.h>
#include <vector>
#include <map>

#include "TestPlayer.h"
#include "Entity/Creature/Humans/Player.h"

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
	float delay;
	//UDP
	sf::UdpSocket UDP_Socket;
	bool shouldSendPacket;
	//TCP
	sf::TcpSocket TCP_Socket;

	sf::Vector2f prevPos, p2Pos;
	sf::Clock clock;

public:
	Network();
	virtual ~Network();

	std::unordered_map<std::string, TestPlayer*> players;
	TestPlayer *p1;
	TestPlayer *p2;

	enum class Action{DELETE, PICK_UP, DROP, CREATE, CRAFT};

	struct EntityState
	{
		Action action;
		sf::Vector2i chunkID;
		unsigned int chunkObjectID;
		unsigned int objectID;
		unsigned int HP;
		std::vector<unsigned int> equippedItems;
	};

	//UDP
	static void UDP_Traffic(Network* network);
	void UDP_Run();
	void UDP_Send(sf::Packet &packet);
	void UDP_Recieve(sf::Packet& packet);

	//TCP
	static void TCP_Traffic(Network* network);
	void  TCP_Run();
};


#endif