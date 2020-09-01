#ifndef ENET_CLIENT
#define ENET_CLIENT

#include "Networking/ENet_networking/ENetwork.h"

class ENetClient : public ENetwork
{
private:
	virtual int init();
	ENetPeer* peer;
	void sendRequestToServer();

	const int requestTickRate = 64/*hz*/; //sends to server at most 64 packages/second
								   //Tickduration = 1000/64 = 15,645ms
protected:
	virtual void* receiveEventsLoop(void);
	virtual void* sendPacketsLoop(void);

public:
	ENetClient(/* args */);
	virtual ~ENetClient();
	int connect();
	virtual int disconnect();
};


#endif //ENET_CLIENT