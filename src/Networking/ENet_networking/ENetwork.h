#ifndef ENETWORK
#define ENETWORK

#include "ENetTestGame.h"

class ENetwork
{
private:
	int initENet();
	virtual int init() = 0;
	bool threadLoopRunning;

protected:
	std::string ID;
	//ENet
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

	//Handle packet-traffic
	typedef ENetTestPlayer::StateType RequestType;
	typedef ENetTestPlayer::State Request;

	enum PacketType { PLAYER_STATE, GAME_STATE, PLAYER_CONNECTED,
					  PLAYER_DISCONNECTED, HOST_DISCONNECTED, GAME_START,
					  GAME_PAUSED, GAME_RESTART, GAME_QUIT};

	const char* extractData(enet_uint8* data);

	//Receive events are the same for client & server
	void handleReceiveEvent(ENetEvent* event);

	//Player disconnection are not similar for client & server
	virtual void handleDisconnectEvent(ENetEvent* event) = 0;

	//Sending a packet to peer are the same for client & server
	void sendPacket(ENetPeer* peer, enet_uint8 channel, const char* data);

	//Network loops in threads
	virtual void receiveEvents() = 0;
	virtual void sendPackets() = 0;
	void* traffic(void);

	//Helper function to "void* traffic(void) needed for threading"
	static void* trafficHelper(void *context)
	{
		return ((ENetwork *)context)->traffic();
	}

	static void printPacketData(const char* data);

public:
	ENetwork(/* args */);
	virtual ~ENetwork();

	int run();
	//Disconnection are not similar for client & server
	virtual int disconnect() = 0;

	//Setters
	inline void setThreadLoopRunning(const bool &state){threadLoopRunning = state;}

	//Getters
	inline const bool& getTheadLoopRunning()const{return threadLoopRunning;}
};


#endif //ENETWORK