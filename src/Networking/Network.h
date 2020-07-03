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
	
	//Players
	TestPlayer* player;
	std::unordered_map<std::string, Network*> players;
	void updatePlayers(const float & dt);
	void drawPlayers(sf::RenderTarget* target);
	void start();
	
	virtual void UDP_send(sf::Packet &packet, sf::IpAddress &address) = 0;
	virtual void UDP_recieve(sf::Packet& packet, sf::IpAddress &address) = 0;
	virtual void TCP_send(sf::Packet &packet) = 0;
	virtual void TCP_recieve(sf::Packet&packet) = 0;
	enum class TCP_type{PLAYER_CONNECTED, PLAYER_LEFT, SERVER_QUIT, GAME_PAUSED};
};

template<typename S, typename N>
sf::Packet& operator<<(sf::Packet& packet, std::unordered_map<S, N>& map)
{
	for(auto i: map)
	{
		packet << i.first << 
		i.second->player->rect.getPosition().x << 
		i.second->player->rect.getPosition().y;
	}
	return packet;
}

template<typename S, typename N>
sf::Packet& operator>>(sf::Packet& packet, std::unordered_map<S, N>& map)
{
    for(auto i: map)
	{
		std::string id;
		Vector2f pos;
		if(packet >> id >> pos.x >> pos.y)
			map[id]->player->p2Pos = pos;
	}
	return packet;
}

#endif