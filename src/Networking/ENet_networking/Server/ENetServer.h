#ifndef ENET_SERVER
#define ENET_SERVER

#include "Networking/ENet_networking/ENetwork.h"

class ENetServer: public ENetwork
{
private:
	virtual bool init();
	const size_t MaxNumberOfPlayers = 32;
	std::unordered_map<ENetPeer* /*peers*/, ENetTestPlayer* /*player*/> peers;

protected:
	virtual void receiveEvents();
	void brodcastPacket(const char* data);

public:
	ENetServer(/* args */);
	virtual ~ENetServer();

	virtual bool disconnect();
};




#endif //ENET_SERVER