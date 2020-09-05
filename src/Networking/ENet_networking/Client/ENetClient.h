#ifndef ENET_CLIENT
#define ENET_CLIENT

#include "Networking/ENet_networking/ENetwork.h"

class ENetClient : public ENetwork
{
private:
	virtual int init();
	ENetPeer* peer;
	DataString* getDataFromRequest(const Request &request);
	void sendRequestToServer(const Request& request);
	std::vector<Request> requestQueue;
	void checkPlayerState();
	void addRequest(const Request &request);

	const int requestTickRate = 64/*hz*/; //sends to server at most N packages/second
								   //Tickduration = 1000/N = Xms

	void addPlayer(const DataVec& playerDataVec);
	void removePlayer(const DataVec& playerDataVec);
	void hostDisconnected(const DataVec& hostDataVec);

protected:
	virtual void handleReceiveEvent(ENetEvent* event);
	virtual void handleDisconnectEvent(ENetEvent* event);
	virtual void receiveEvents();
	virtual void sendPackets();

public:
	ENetClient(/* args */);
	virtual ~ENetClient();

	int connect();
	virtual int disconnect();
};


#endif //ENET_CLIENT