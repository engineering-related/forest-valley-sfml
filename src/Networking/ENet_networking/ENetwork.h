#ifndef ENETWORK
#define ENETWORK
#include "ENetTestPlayer.h"

class ENetwork
{
private:
	bool initENet();
	void initTestPlayer();
	virtual bool init() = 0;

protected:
	//ENet
	ENetAddress address;
    ENetHost* host;
    ENetEvent event;
	enet_uint32 delay;
	const size_t channels = 3;
	//Game
	sf::RenderWindow* window;
	float dt;
	sf::Clock gameClock;
	//Players
	ENetTestPlayer* player;
	std::unordered_map<std::string /*key-id*/, ENetTestPlayer* /*players*/> players;
	//Handle packet-traffic
	static void sendPacket(ENetPeer* peer, enet_uint8 channel, const char* data);
	virtual void receiveEvents() = 0;

public:
	ENetwork(/* args */);
	virtual ~ENetwork();

	void updatePlayers(const float & dt);
	void drawPlayers(sf::RenderTarget* target);
	void run();
	virtual bool disconnect() = 0;
};


#endif //ENETWORK