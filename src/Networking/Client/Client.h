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
		std::string id;
		std::string text;

	public:
		std::string sendIP;
		Client(/* args */);
		virtual ~Client();

		void connectToServer();
};

#endif