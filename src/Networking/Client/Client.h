#ifndef NETWORK_CLIENT
#define NETWORK_CLIENT

#include "Networking/Network.h"

class Client: public Network
{
	private:
		/* data */
	protected:
		//UDP

		//TCP

	public:
		Client();
		virtual ~Client();

		void TCP_connect();
		void UDP_connect();
		void connectToServer();
		void disconnectFromServer();
};

#endif