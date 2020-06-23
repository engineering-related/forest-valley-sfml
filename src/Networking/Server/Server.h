#ifndef NETWORK_SERVER
#define NETWORK_SERVER

#include "Networking/Network.h"

class Server: public Network
{
	private:

	protected:

	public:
		Server(/* args */);
		virtual ~Server();

		void init();
};

#endif