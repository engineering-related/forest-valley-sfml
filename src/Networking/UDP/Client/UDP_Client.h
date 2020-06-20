#ifndef NETWORK_UDP_CLIENT
#define NETWORK_UDP_CLIENT

#include "Networking/UDP/UDP.h"

class UDP_Client: public UDP
{
private:

protected:

public:
	void connectToServer();
	UDP_Client();
	~UDP_Client();

};
#endif