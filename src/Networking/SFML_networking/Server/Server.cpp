#include "Server.h"

Server::Server(/* args */)
{
	this->thread = new sf::Thread(&Server::run, this);
	this->tickRate = sf::milliseconds(this->delay);
}

Server::~Server()
{

}

void Server::checkNewClientConnection()
{
	if(this->selector.isReady(this->TCP_listener))
	{
		Client *client = new Client;
		if(this->TCP_listener.accept(client->TCP_Socket) == sf::Socket::Done)
		{
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
			allPlayers[id] = this;
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
			serverSendPacket << (sf::Int32)TCP_type::PLAYER_CONNECTED <<
			client->id  <<
			pos.x << pos.y <<
			r << g << b;

			for (auto p : players)
			{
				p.second->TCP_Socket.send(serverSendPacket);
			}
			//Add player to the server
			this->globalMutex.lock();
			this->players[client->id] = client;
			this->globalMutex.unlock();
		}
		else
		{
			delete client;
		}
	}
}

void Server::disconnectClient(Network* client)
{
	globalMutex.lock();
	//Remove client for map
	this->players.erase(client->id);
	this->selector.remove(client->TCP_Socket);

	//Update for the rest of the server
	sf::Packet serverSendPacket;
	serverSendPacket << (sf::Int32)TCP_type::PLAYER_LEFT << client->id;

	for (auto p : players)
	{
		p.second->TCP_Socket.send(serverSendPacket);
	}
	//Delete the client from memory
	std::cout << "(TCP) " << client->id << " disconnected with ip: " + client->localIp.toString() << std::endl;
	delete client;
	globalMutex.unlock();
}

void Server::UDP_send(Network* n, Packet &packet, sf::IpAddress &address)
{
	//Send packet
	this->globalMutex.lock();
	packet << id << this->player->rect.getPosition().x << this->player->rect.getPosition().y;
	for(auto i: players)
	{
		if(n != i.second)
		{
			packet << i.first <<
			i.second->player->rect.getPosition().x <<
			i.second->player->rect.getPosition().y;
		}
	}
	this->globalMutex.unlock();
	this->UDP_Socket.send(packet, address, this->port);
}

void Server::UDP_recieve(sf::Packet& packet, sf::IpAddress &address)
{
	//Receive packet
	this->UDP_Socket.receive(packet, address, this->port);
	std::string id;
	Vector2f pos;
	if (packet >> id >> pos.x >> pos.y)
	{
		this->globalMutex.lock();
		players[id]->player->p2Pos = pos;
		this->player->prevPos = this->player->rect.getPosition();
		this->globalMutex.unlock();
	}
}

void Server::TCP_send(Network* n, sf::Packet &packet)
{
	//Testing with TCP
	sf::Packet TCP_packet;
	TCP_packet << (sf::Int32)TCP_type::SERVER_QUIT;
	n->TCP_Socket.send(TCP_packet);
}

void Server::TCP_recieve(sf::Packet &packet)
{
	//Receive data from all the clients about their state
}

void Server::update(Server* server)
{
	if(!server->selector.isReady(server->TCP_listener))
	{
		sf::IpAddress clientAdress;
		sf::Packet packet;
		//Check connection
		for(auto i: server->players)
		{
			//Check client connection
			if (server->selector.isReady(i.second->TCP_Socket))
            {
				if(i.second->TCP_Socket.receive(packet) == sf::Socket::Disconnected)
				{
					server->disconnectClient(i.second);
					break;
				}
			}
		}

		if(server->clock.getElapsedTime().asMilliseconds() >= server->delay)
		{
			server->clock.restart().asMilliseconds();
			for(auto i: server->players)
			{
				server->UDP_send(i.second, packet, i.second->localIp);
			}
		}
		server->UDP_recieve(packet, clientAdress);
	}
}

void Server::UDP_init()
{
	UDP_Socket.bind(port);
	selector.add(UDP_Socket);
	UDP_Socket.setBlocking(false);
}

void Server::TCP_init()
{
	this->TCP_listener.listen(port);
	this->selector.add(TCP_listener);
	this->TCP_Socket.setBlocking(false);
}


void Server::init()
{
	this->UDP_init();
	this->TCP_init();
	std::cout << "Enter id: ";
	std::cin >> id;
	this->id = id;
}

void Server::done()
{
	for(auto i: this->players)
	{
		sf::Packet packet;
		this->TCP_send(i.second, packet);
	}
}

void Server::run(Server* server)
{
	server->clock.restart().asMilliseconds();
	while(!server->quit)
	{
		if(server->selector.wait())
		{
			server->checkNewClientConnection();
			server->update(server);
		}
	}
	server->done();
}
