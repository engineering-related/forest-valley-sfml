#ifndef ENET_SERVER
#define ENET_SERVER

#include "Networking/ENet_networking/ENetwork.h"

class ENetServer: public ENetwork
{
private:
	virtual int init();
	const size_t MaxNumberOfPlayers = 32;
	std::unordered_map<ENetPeer* /*peers*/, ENetTestPlayer* /*player*/> peers;

	const int tickRate = 64/*hz*/; //sends to clients at most 64 packages/second
								   //Tickduration = 1000/64 = 15,645ms
	void addPlayer(ENetPeer* peer);

protected:
	void handleConnectionEvent(ENetEvent* event);
	virtual void handleDisconnectEvent(ENetEvent* event);

	virtual void receiveEvents();
	virtual void sendPackets();

	//Send a packet to all peers
	void brodcastPacket(const char* data);

public:
	ENetServer(/* args */);
	virtual ~ENetServer();

	virtual int disconnect();
};

#endif //ENET_SERVER