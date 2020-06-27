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
		std::string publicSendIp;
		std::string localSendIp;
		Client(/* args */);
		virtual ~Client();

		void connectToServer();
};

#endif