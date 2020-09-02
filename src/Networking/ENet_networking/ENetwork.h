#ifndef ENETWORK
#define ENETWORK
#include "ENetTestPlayer.h"

class ENetwork
{
private:
	int initENet();
	void initTestPlayer();
	virtual int init() = 0;
	bool threadsLoopRunning;
	bool gameLoopRunning;

protected:
	//ENet
	ENetAddress address;
    ENetHost* host;
    ENetEvent event;
	const enet_uint32 NONE_BLOCKING = 0;
	const size_t channels = 3;
	sf::Clock clock;

	//Threads
	pthread_mutex_t lock;
	pthread_t networkThread;

	inline void setThreadsLoopRunning(const bool &state){threadsLoopRunning = state;}
	inline const bool& getTheadsLoopRunning()const{return threadsLoopRunning;}

	//Game
	sf::RenderWindow* window;
	float dt;
	sf::Clock gameClock;

	inline void setGameLoopRunning(const bool &state){gameLoopRunning = state;}
	inline const bool& getGameLoopRunning()const{return gameLoopRunning;}

	//Players
	ENetTestPlayer* player;
	std::unordered_map<std::string /*key-id*/, ENetTestPlayer* /*players*/> players;

	//Handle packet-traffic
	typedef ENetTestPlayer::StateType RequestType;
	typedef ENetTestPlayer::State Request;

	enum PacketType { SERVER_DATA, CLIENT_DATA, PLAYER_CONNECTED,
					  PLAYER_DISCONNECTED, HOST_DISCONNECTED, GAME_START,
					  GAME_PAUSED, GAME_RESTART, GAME_QUIT};

	const char* extractData(enet_uint8* data);
	void handleReceiveEvent(ENetEvent* event);
	virtual void handleDisconnectEvent(ENetEvent* event) = 0;

	void sendPacket(ENetPeer* peer, enet_uint8 channel, const char* data);

	//Network loops in threads
	virtual void receiveEvents() = 0;
	virtual void sendPackets() = 0;
	void* traffic(void);
	static void* trafficHelper(void *context)
	{
		return ((ENetwork *)context)->traffic();
	}

public:
	ENetwork(/* args */);
	virtual ~ENetwork();
	void updatePlayers(const float & dt);
	void drawPlayers(sf::RenderTarget* target);
	int run();
	virtual int disconnect() = 0;
};


#endif //ENETWORK