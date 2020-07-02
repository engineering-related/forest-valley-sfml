#include "Server.h"

Server::Server(/* args */)
{
	this->thread = new sf::Thread(&Server::run, this);
	this->thread2 = new sf::Thread(&Server::update, this);
	this->tickRate = sf::milliseconds(this->delay);
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
		sf::Int32 r, g, b;
		sf::Vector2f pos;
		//Receve packet about the new player information
		if(client->TCP_Socket.receive(packet) == sf::Socket::Done)
		{
			packet >> client->id >> pos.x >> pos.y >>
			r >> g >> b;
		}
		sf::Color color(r, g, b);

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
				std::cout << "(UDP) connected with message: " << buffer << std::endl;
				receviedIp = true;
				client->localIp = sender;
			}
		}
		client->player->rect.setPosition(pos);
		client->player->rect.setFillColor(color);

		std::cout << "(TCP) connected with ip: " << client->localIp << std::endl;

		this->selector.add(client->TCP_Socket);

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
		r << g << b;

		for (auto p : players)
		{
			if(selector.isReady(p.second->TCP_Socket))
			{
				p.second->TCP_Socket.send(serverSendPacket);
			}
		}
		//Add player to the server
		this->globalMutex.lock();
		this->players[client->id] = client;
		clients[client->localIp.toString()] = client;
		this->globalMutex.unlock();
	}
}

void Server::update(Server* server)
{
	while(!server->quit)
	{
		for(auto i: server->clients)
		{
			sf::Packet packet, sendPacket, serverPacket;
			sf::IpAddress clientAdress;

			server->UDP_Socket.receive(packet, clientAdress, server->port);

			sf::Vector2f pos;
			packet >> pos.x >> pos.y;
			std::cout << pos.x << " " << pos.y << std::endl;
 			sendPacket << server->clients[i.first]->id << pos.x << pos.y;
			serverPacket << server->id << server->player->rect.getPosition().x << server->player->rect.getPosition().y;

			server->UDP_recieve(sendPacket, false);
			server->UDP_Socket.send(serverPacket, clientAdress, server->port);
			for(auto j: server->clients)
			{
				if(j.second != i.second)
					server->UDP_Socket.send(sendPacket, j.second->localIp, server->port);
			}
		}
	}
}



void Server::UDP_init()
{
	UDP_Socket.bind(port);
	//selector.add(UDP_Socket);
	UDP_Socket.setBlocking(false);
}

void Server::TCP_init()
{
	this->TCP_listener.listen(port);
	this->selector.add(TCP_listener);
}


void Server::init()
{
	this->UDP_init();
	this->TCP_init();
	std::cout << "Enter id: ";
	std::cin >> id;
	this->id = id;
}

void Server::run(Server* server)
{
	while(!server->quit)
	{
		if(server->selector.wait(server->tickRate))
		{
			server->listenConnections();
		}
	}
}