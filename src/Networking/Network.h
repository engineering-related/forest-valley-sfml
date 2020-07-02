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
	sf::Thread* thread;

public:
	Network();
	virtual ~Network();

	sf::Mutex globalMutex;
	sf::IpAddress publicSendIp;
	sf::IpAddress localSendIp;
	sf::IpAddress localIp;
	std::string id;
	unsigned short port;
	bool quit;
	float dt;
	sf::Clock gameClock;
	float delay;
	//UDP
	sf::UdpSocket UDP_Socket;
	bool UDP_send_packet;
	bool TCP_send_packet;
	//TCP
	sf::TcpSocket TCP_Socket;
	sf::Clock clock;

	//Traffic
	static void traffic(Network* network);

	//Players
	TestPlayer* player;
	std::unordered_map<std::string, Network*> players;
	void updatePlayers(const float & dt);
	void drawPlayers(sf::RenderTarget* target);

	//UDP
	void start();
	void UDP_send(std::string &id, sf::Packet &packet, sf::IpAddress &address);
	bool UDP_recieve(sf::Packet& packet, sf::IpAddress &address);

	//TCP
	enum class TCP_type{PLAYER_CONNECTED, PLAYER_LEFT, SERVER_QUIT, GAME_PAUSED};
	void TCP_send(sf::Packet &packet);
	void TCP_recieve(sf::Packet&packet);

	//FUNCTIONS
	void addPlayer(sf::Packet &packet);
	void removePlayer(sf::Packet &packet);
};


#endif