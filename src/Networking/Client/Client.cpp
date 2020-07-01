#include "Client.h"

Client::Client(/* args */)
{
	this->publicSendIp =  "87.96.222.250"; //"78.72.205.138";
	this->localSendIp = "192.168.1.104";

	this->thread = new sf::Thread(&Network::traffic, this);
}

Client::~Client()
{

}

void Client::TCP_connect()
{

}

void Client::UDP_connect()
{
	UDP_Socket.bind(port);
	std::string message = "Hi, I am " + id;
	UDP_Socket.send(message.c_str(), message.size() + 1, localSendIp, port);
	//UDP_Socket.setBlocking(false);
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
		Network* n = new Network();
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

