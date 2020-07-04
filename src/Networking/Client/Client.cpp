#include "Client.h"

Client::Client(/* args */)
{
	this->publicSendIp =  "87.96.222.250"; //"78.72.205.138";
	this->localSendIp = "192.168.1.104";

	this->thread = new sf::Thread(&Client::traffic, this);
}

Client::~Client()
{

}

void Client::TCP_connect()
{

}

void Client::traffic(Client* client)
{
	client->clock.restart().asMilliseconds();
	while (!client->quit)
	{
		sf::Packet packet;
		if (client->clock.getElapsedTime().asMilliseconds() >= client->delay)
		{
			client->clock.restart().asMilliseconds();
			client->UDP_send(client, packet, client->localSendIp);
			client->UDP_recieve(packet, client->publicSendIp);
		}
		client->TCP_recieve(packet);
	}
}

void Client::UDP_send(Network* n, sf::Packet &packet, sf::IpAddress &address)
{
	this->globalMutex.lock();
	if (n->player->prevPos != n->player->rect.getPosition())
		packet << n->id << n->player->rect.getPosition().x << n->player->rect.getPosition().y;
	this->globalMutex.unlock();

	this->UDP_Socket.send(packet, address, this->port);
}

void Client::UDP_recieve(sf::Packet& packet, sf::IpAddress &address)
{
	//Receive packet
	this->UDP_Socket.receive(packet, address, this->port);
	this->globalMutex.lock();
	for(auto i: players)
	{
		std::string id;
		Vector2f pos;
		if(packet >> id >> pos.x >> pos.y)
			players[id]->player->p2Pos = pos;
	}
	this->globalMutex.unlock();
}


void Client::TCP_send(Network* n, sf::Packet &packet)
{
	this->globalMutex.lock();

	this->globalMutex.unlock();
}


void Client::TCP_recieve(sf::Packet &packet)
{
	this->TCP_Socket.receive(packet);
	sf::Int32 type;
	if(packet >> type)
	{
		switch ((TCP_type)type)
		{
		case TCP_type::PLAYER_CONNECTED:
			this->addPlayer(packet);
			break;
		case TCP_type::PLAYER_LEFT:
			this->removePlayer(packet);
			break;
		case TCP_type::SERVER_QUIT:
			quit = true;
			break;
		case TCP_type::GAME_PAUSED:
			//Pause the game...
			break;
		default:
			break;
		}
	}
}

void Client::addPlayer(sf::Packet &packet)
{
	std::string id;
	std::string cLocalIp;
	sf::Vector2f pos;
	sf::Int32 r, g, b;
	packet >> id >>
			  pos.x >> pos.y >>
	          r >> g >> b;

	Client* p = new Client();
	sf::Color color(r, g, b);
	p->id = id;
	p->player->rect.setPosition(pos);
	p->player->rect.setFillColor(color);
	this->globalMutex.lock();
	this->players[id] = p;
	this->globalMutex.unlock();
}

void Client::removePlayer(sf::Packet &packet)
{
	std::string id;
	packet >> id;
	Network* client;
	client = this->players[id];
	this->players.erase(id);
	delete client;
}

void Client::UDP_connect()
{
	UDP_Socket.bind(port);
	UDP_Socket.setBlocking(false);
	std::string message = "Hi, I am " + id;
	UDP_Socket.send(message.c_str(), message.size() + 1, localSendIp , port);
}

void Client::connectToServer()
{
	//Connect to the server with an id
	std::cout << "Enter id: ";
	std::cin >> id;
	TCP_Socket.connect(localSendIp, port);
	sf::Packet sendPacket;
	sf::Int32 r = player->rect.getFillColor().r;
	sf::Int32 g = player->rect.getFillColor().g;
	sf::Int32 b = player->rect.getFillColor().b;
	sendPacket << id << player->rect.getPosition().x << player->rect.getPosition().y <<
	r <<
	g <<
	b;
	TCP_Socket.send(sendPacket);
	UDP_connect();
	//Get a packet back about the server state
	sf::Packet recievePacket;
	TCP_Socket.receive(recievePacket);
	int serverSize;
	recievePacket >> serverSize;
	for (size_t i = 0; i < (size_t)serverSize; i++)
	{
		Client* n = new Client();
		sf::Int32 r, g, b;
		sf::Vector2f pos;
		recievePacket >> n->id >>
					     pos.x >> pos.y >>
						 r >> g >> b;
		sf::Color color(r, g, b);
		n->player->rect.setPosition(pos);
		n->player->rect.setFillColor(color);
		players[n->id] = n;
	}

	TCP_Socket.setBlocking(false);
}
