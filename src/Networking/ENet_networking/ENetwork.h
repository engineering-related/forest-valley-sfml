#ifndef ENETWORK
#define ENETWORK

///////////////////////////////////
#define SERVER_IP "192.168.1.104"
#define PORT 24474
///////////////////////////////////

#include "ENetTestGame.h"

class ENetwork
{
private:
	int initENet();
	virtual int init() = 0;

	bool threadLoopRunning;
	const size_t keyCharacterLength = 8;
	bool shouldDissconnect;

protected:
	//Typedefs
	typedef ENetTestPlayer::StateType RequestType;
	typedef ENetTestPlayer::State Request;
	typedef std::vector<std::string> DataVec;
	typedef const char DataString;

	//Package types
	enum PacketType { PLAYER_STATE, GAME_STATE, GAME_DATA, PLAYER_CONNECTED,
					  PLAYER_DISCONNECTED, HOST_DISCONNECTED, GAME_START,
					  GAME_PAUSED, GAME_RESTART, GAME_QUIT};

	//ENet
	std::string ENetID;
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

	//Packets
	DataVec extractData(enet_uint8* data);

	//Events
	virtual void handleReceiveEvent(ENetEvent* event) = 0;
	virtual void handleDisconnectEvent(ENetEvent* event) = 0;

	//WARNING: Should convert to bytes later
	void sendPacket(ENetPeer* peer, enet_uint8 channel, DataString* data);

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
	static void printPacketData(DataVec dataVec);

public:
	ENetwork(/* args */);
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