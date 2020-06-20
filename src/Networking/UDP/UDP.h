#ifndef NETWORK_UDP
#define NETWORK_UDP

#include "Networking/Network.h"

class UDP: public Network
{
private:

protected:
	sf::UdpSocket socket;

public:
	UDP();
	virtual ~UDP();
	static void handlePacketTraffic(UDP* UDP_Network);
};

#endif