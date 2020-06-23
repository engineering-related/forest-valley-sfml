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
		Client(/* args */);
		virtual ~Client();

		void connectToServer();
};

#endif