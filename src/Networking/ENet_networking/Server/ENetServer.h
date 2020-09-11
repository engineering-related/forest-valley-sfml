#ifndef ENET_SERVER
#define ENET_SERVER

#include "Networking/ENet_networking/ENetwork.h"

class ENetServer: public ENetwork
{
private:
	bool receivedDuringTick;
	virtual int init();
	const size_t MaxNumberOfPlayers = 32;
	std::unordered_map<ENetPeer* /*Peer*/, std::string /*ENetID*/> peers;

	const int tickRate = 1000/*hz*/; //sends to clients at most N packages/second
								   //Tickduration = 1000/N = Xms

	//PlayerStates
	void handlePlayerState(sf::Packet& packet);
	void evaluatePlayerState(const std::string& ENetID, ENetTestPlayer::State& state);
	void updatePlayerState(const std::string& ENetID, ENetTestPlayer::State& state);

	//Server changes
	void addPlayerToServer(sf::Packet& packet, ENetPeer* peer);
	void removePlayerFromServer(ENetPeer* peer);

	//WARNING: Undefined, could be useful later
	void disconnectPlayer(ENetPeer* peer);
	void banPlayer(ENetPeer* peer);

protected:
	//WARNING: Undefined, Evaluation of packet (anit-cheat and saftey)
	void evaluateReceivedPacket(sf::Packet &packet);

	//Connection evetns
	void handleConnectionEvent(ENetEvent* event);

	//Virtual protected functions
	virtual void handleReceiveEvent(ENetEvent* event);
	virtual void handleDisconnectEvent(ENetEvent* event);
	virtual void receiveEvents();
	virtual void sendPackets();

	//Send a packet to all peers
	void brodcastPacket(const size_t& channel, const sf::Packet& packet);

public:
	ENetServer(/* args */);
	virtual ~ENetServer();

	virtual int disconnect();
};

#endif //ENET_SERVER