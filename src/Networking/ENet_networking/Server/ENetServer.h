#ifndef ENET_SERVER
#define ENET_SERVER

#include "Networking/ENet_networking/ENetwork.h"

class ENetServer: public ENetwork
{
private:
	virtual int init();
	const size_t MaxNumberOfPlayers = 32;
	std::unordered_map<ENetPeer* /*Peer*/, std::string/*ENetID*/> peers;

	const int tickRate = 32/*hz*/; //sends to clients at most N packages/second
								   //Tickduration = 1000/N = Xms

	//PlayerStates
	void handlePlayerState(const DataVec& playerDataVec);
	void evaluatePlayerState(const DataVec& playerDataVec);
	void updatePlayerState(const DataVec& playerDataVec);

	//Handeling connections
	void addPlayerToServer(const DataVec& playerDataVec, ENetPeer* peer);
	void removePlayerFromServer(ENetPeer* peer);

protected:
	bool receivedDuringTick;
	virtual void handleReceiveEvent(ENetEvent* event);
	//Evaluation of packet (no buffer overflows or wrong types of data sent to the server)
	void evaluateReceivedPacket(ENetPacket* packet);
	void handleConnectionEvent(ENetEvent* event);
	virtual void handleDisconnectEvent(ENetEvent* event);

	virtual void receiveEvents();
	virtual void sendPackets();


	//Send a packet to all peers
	void brodcastPacket(DataString* data, const size_t& channel);

public:
	ENetServer(/* args */);
	virtual ~ENetServer();

	virtual int disconnect();
};

#endif //ENET_SERVER