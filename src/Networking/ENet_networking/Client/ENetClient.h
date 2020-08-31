#ifndef ENET_CLIENT
#define ENET_CLIENT

#include "Networking/ENet_networking/ENetwork.h"

class ENetClient : public ENetwork
{
private:
	virtual bool init();
	ENetPeer* peer;

protected:
	virtual void receiveEvents();

public:
	ENetClient(/* args */);
	virtual ~ENetClient();
	bool connect();
	virtual bool disconnect();
};


#endif //ENET_CLIENT