#include "Server.h"

Server::Server(/* args */)
{
	this->thread = new sf::Thread(&Server::run, this);
}

Server::~Server()
{

}

void Server::listenConnections()
{
	if(this->selector.isReady(this->TCP_listener))
	{
		Client *client = new Client;
		this->TCP_listener.accept(client->TCP_Socket);
		sf::Packet packet;
		sf::Color color;
		sf::Vector2f pos;
		//Receve packet about the new player information
		if(client->TCP_Socket.receive(packet) == sf::Socket::Done)
		{
			packet >> client->id >> pos.x >> pos.y >>
			color.r >> color.g >> color.b;
		}

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
				std::cout << "(UDP) connected with ip " << buffer << std::endl;
				receviedIp = true;
				client->localIp = sender;
			}
		}
		client->player->rect.setPosition(pos);
		client->player->rect.setFillColor(color);


		std::cout << "(TCP) "<< client->id << " connected with ip: " << client->localIp << std::endl;

		this->selector.add(client->TCP_Socket);
		this->selector.add(client->UDP_Socket);

		sf::Packet sendPacket;
		std::unordered_map<std::string, Network*> allPlayers = players;
		allPlayers[this->id] = this;
		sendPacket << (int)allPlayers.size();
		//Send back the serverstate to the connected player
		for (auto p : allPlayers)
		{
			sendPacket << p.first << p.second->player->rect.getPosition().x << p.second->player->rect.getPosition().y <<
			(sf::Int32)p.second->player->rect.getFillColor().r <<
			(sf::Int32)p.second->player->rect.getFillColor().g <<
			(sf::Int32)p.second->player->rect.getFillColor().b;
		}

		client->TCP_Socket.send(sendPacket);

		//Update for the rest of the server
		sf::Packet serverSendPacket;
		serverSendPacket << (int)TCP_type::PLAYER_CONNECTED <<
		client->id << client->localIp.toString() <<
		pos.x << pos.y <<
		(sf::Int32)color.r << (sf::Int32)color.g << (sf::Int32)color.b;

		for (auto p : players)
		{
			if(selector.isReady(p.second->TCP_Socket))
			{
				p.second->TCP_Socket.send(serverSendPacket);
			}
		}
		//this->addPlayer(serverSendPacket);
	}
}

void Server::update()
{
	if(!this->selector.isReady(TCP_listener))
	{
		for(auto i: players)
		{
			if(selector.isReady(i.second->UDP_Socket))
			{
				sf::Packet packet, sendPacket, serverPacket;
				if(i.second->UDP_Socket.receive(packet, i.second->localIp, port) == sf::Socket::Done)
				{
					sf::Vector2f pos;
					packet >> pos.x >> pos.y;
					sendPacket << i.second->id << pos.x << pos.y;
					serverPacket << this->id << this->player->rect.getPosition().x << this->player->rect.getPosition().y;

					this->UDP_recieve(sendPacket, false);
					i.second->UDP_Socket.send(serverPacket, sf::IpAddress::getLocalAddress(), port);

					for(auto j: players)
					{
						if(j.second != i.second)
						{
							j.second->UDP_Socket.send(sendPacket, sf::IpAddress::getLocalAddress(), port);
						}
					}
				}
			}
		}
	}
}



void Server::UDP_init()
{
	UDP_Socket.bind(port);
	UDP_Socket.setBlocking(false);
	this->selector.add(this->UDP_Socket);
}

void Server::TCP_init()
{
	this->TCP_listener.listen(port);
	this->selector.add(TCP_listener);
}


void Server::init()
{
	this->TCP_init();
	this->UDP_init();
	std::cout << "Enter id: ";
	std::cin >> id;
	this->id = id;
}

void Server::run(Server* server)
{
	while(!server->quit)
	{
		//Maybe add delay after!
		if(server->selector.wait())
		{
			server->listenConnections();
			server->update();
		}
	}
}