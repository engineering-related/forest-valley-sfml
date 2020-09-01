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
	pthread_t recieveThread;
	pthread_t sendThread;

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
	typedef ENetTestPlayer::Action Request;

	void sendPacket(ENetPeer* peer, enet_uint8 channel, const char* data);

	//Network loops in threads
	virtual void* receiveEventsLoop(void) = 0;
	static void* recieveEventsHelper(void *context)
	{
		return ((ENetwork *)context)->receiveEventsLoop();
	}

	virtual void* sendPacketsLoop(void) = 0;
	static void* sendPacketsHelper(void *context)
	{
		return ((ENetwork *)context)->sendPacketsLoop();
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