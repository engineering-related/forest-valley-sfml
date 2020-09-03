#ifndef ENET_CLIENT
#define ENET_CLIENT

#include "Networking/ENet_networking/ENetwork.h"

class ENetClient : public ENetwork
{
private:
	virtual int init();
	ENetPeer* peer;
	const char* getDataFromRequest(const Request &request);
	void sendRequestToServer(const Request& request);
	std::vector<Request> requestQueue;
	void checkPlayerState();
	void addRequest(const Request &request);

	const int requestTickRate = 64/*hz*/; //sends to server at most 64 packages/second
								   //Tickduration = 1000/64 = 15,645ms

protected:
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