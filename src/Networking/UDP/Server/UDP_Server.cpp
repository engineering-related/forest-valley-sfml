#include "UDP_Server.h"

UDP_Server::UDP_Server()
{

}

UDP_Server::~UDP_Server()
{

}


void UDP_Server::init()
{
	char connectionType;

	socket.bind(port);
	socket.setBlocking(false);

	bool receviedIp = false;
	while (!receviedIp)
	{
		// Receive a message from anyone
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);
		if (received > 0)
		{
			std::cout << received << std::endl;
			std::cout << sender.toString() << " connected with " << buffer << std::endl;
			receviedIp = true;
			sendIp = sender;
		}
	}

}