#include "Server.h"

Server::Server(/* args */)
{
}

Server::~Server()
{
}

void Server::init()
{
	char connectionType;

	UDP_Socket.bind(port);
	UDP_Socket.setBlocking(false);

	bool receviedIp = false;
	while (!receviedIp)
	{
		// Receive a message from anyone
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		UDP_Socket.receive(buffer, sizeof(buffer), received, sender, port);
		if (received > 0)
		{
			std::cout << received << std::endl;
			std::cout << sender.toString() << " connected with " << buffer << std::endl;
			receviedIp = true;
			sendIp = sender;
		}
	}
}
