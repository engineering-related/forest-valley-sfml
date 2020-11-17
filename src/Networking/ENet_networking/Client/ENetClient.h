#ifndef ENET_CLIENT
#define ENET_CLIENT

#include "Networking/ENet_networking/ENetwork.h"

class ENetClient : public ENetwork
{
private:
	virtual int init();
	ENetPeer* peer;
	sf::Packet getDataFromRequest(const Request &request);
	void sendRequestToServer(const Request& request);
	std::vector<Request> requestQueue;
	void checkPlayerState();
	void addRequest(const Request &request);

	const int requestTickRate = 1000/*hz*/; //sends to server at most N packages/second
								   //Tickduration = 1000/N = Xms

	const int connectionTimeout = 5000; //ms
	const int disconnectionTimeout = 3000; //ms

	void addPlayer(sf::Packet& packet);
	void removePlayer(sf::Packet& packet);
	void hostDisconnected(sf::Packet& packetc);
	void changePlayerID(sf::Packet& packet);

protected:
	//Virtual protected functions
	virtual void handleReceiveEvent(ENetEvent* event);
	virtual void handleDisconnectEvent(ENetEvent* event);
	virtual void receiveEvents();
	virtual void sendPackets();

public:
	ENetClient(const char* serverIP, const short &port);
	virtual ~ENetClient();

	int connect();
	virtual int disconnect();
};


#endif //ENET_CLIENT