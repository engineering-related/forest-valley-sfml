#include "Client.h"

Client::Client(/* args */)
{
	this->sendIp =  "87.96.222.250"; //"78.72.205.138";
	this->text = "";
}

Client::~Client()
{

}

void Client::connectToServer(){
	//TCP
	std::cout << "Enter id: ";
	std::cin >> this->id;
	TCP_Socket.connect(sendIP, port);
	sf::Packet packet;
	packet << this->id;
	TCP_Socket.send(packet);

	//UDP
	UDP_Socket.bind(port);
	UDP_Socket.setBlocking(false);
	std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
	UDP_Socket.send(message.c_str(), message.size() + 1, sendIp, port);
}
