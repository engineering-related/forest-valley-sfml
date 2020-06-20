#ifndef NETWORK_UDP_SERVER
#define NETWORK_UDP_SERVER

#include "Networking/UDP/UDP.h"


class UDP_Server: public UDP
{
private:

protected:

public:
	void init();
	UDP_Server();
	~UDP_Server();
};

#endif