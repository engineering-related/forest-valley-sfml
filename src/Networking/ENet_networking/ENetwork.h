#ifndef ENETWORK
#define ENETWORK

#include "ENetTestGame.h"

class ENetwork
{
private:
	int initENet();
	virtual int init() = 0;

	bool threadLoopRunning;
	bool shouldDissconnect;

protected:
	//Typedefs
	typedef ENetTestPlayer::StateType RequestType;
	typedef ENetTestPlayer::State Request;

	//Package types
	enum PacketType { PLAYER_STATE, GAME_STATE, GAME_DATA, PLAYER_CONNECTED,
					  PLAYER_DISCONNECTED, PLAYER_CHANGED_ID, HOST_DISCONNECTED, GAME_START,
					  GAME_PAUSED, GAME_RESTART, GAME_QUIT};

	//WARNING: Should convert to bytes later
	void sendPacket(ENetPeer* peer, enet_uint8 channel, const sf::Packet& packet);

	//ENet
	const char* serverIP;
	const enet_uint16* port;
	sf::Uint16 ENetID; //65,535 max objects( 2^16 - 1)
	ENetAddress address;
    ENetHost* host;
    ENetEvent event;
	const enet_uint32 NONE_BLOCKING = 0;
	const size_t channels = 3;
	sf::Clock clock;

	//Threads
	pthread_t networkThread;

	//Game
	ENetTestGame* game;

	//Events
	virtual void handleReceiveEvent(ENetEvent* event) = 0;
	virtual void handleDisconnectEvent(ENetEvent* event) = 0;

	//Network loops in threads
	virtual void receiveEvents() = 0;
	virtual void sendPackets() = 0;

	//Packet traffic
	void* traffic(void);

	//Helper function to "void* traffic(void) needed for threading"
	static void* trafficHelper(void *context)
	{
		return ((ENetwork *)context)->traffic();
	}

public:
	ENetwork(const char* serverIP, const short &port);
	virtual ~ENetwork();

	int run();
	virtual int disconnect() = 0;
	inline void setShouldDisconnect(const bool& state) {shouldDissconnect = state;}

	//Setters
	inline void setThreadLoopRunning(const bool &state){threadLoopRunning = state;}

	//Getters
	inline const bool& getTheadLoopRunning()const{return threadLoopRunning;}
};


#endif //ENETWORK