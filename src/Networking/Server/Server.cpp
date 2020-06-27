#include "Server.h"

Server::Server(/* args */)
{
}

Server::~Server()
{
	//Remove all clients
	for(std::vector<sf::TcpSocket*>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
		delete *it;
}

void Server::UDP_init()
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

void Server::TCP_init()
{
	if(this->TCP_listener.listen(port) != sf::Socket::Done)
	{

	};

	this->selector.add(TCP_listener);





	while(!this->quit)
	{
		if(this->selector.wait())
		{
			if(this->selector.isReady(this->TCP_listener))
			{
				sf::TcpSocket *socket = new sf::TcpSocket;
				this->TCP_listener.accept(*socket);
				sf::Packet packet;
				std::string id;
				if(socket->receive(packet) == sf::Socket::Done)
					packet >> id;

				std::cout << id << " connected to server" << std::endl;
				this->clients.push_back(socket);
				this->selector.add(*socket);
			}
			else
			{
				for(size_t i= 0; i < this->clients.size(); i++)
				{
					if(selector.isReady(*clients[i]))
					{
						sf::Packet packet, sendPacket;
						if(this->clients[i]->receive(packet) == sf::Socket::Done)
						{
							std::string text;
							packet >> text;
							sendPacket << text;
							for(size_t j = 0; j < this->clients.size(); j++)
							{
								if(i != j)
								{
									clients[j]->send(sendPacket);
								}
							}
						}
					}
				}
			}
		}
	}
}

void Server::init()
{
	this->TCP_init();
	this->UDP_init();
}