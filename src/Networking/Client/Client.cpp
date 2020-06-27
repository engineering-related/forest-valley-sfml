#include "Client.h"

Client::Client(/* args */)
{
	std::cout << sf::IpAddress::getLocalAddress().toString() << std::endl;
	this->publicSendIp =  "87.96.222.250"; //"78.72.205.138";
	this->localSendIp = "192.168.1.104";
	this->text = "";
}

Client::~Client()
{

}

void Client::connectToServer(){
	//TCP
	std::cout << "Enter id: ";
	std::cin >> id;
	TCP_Socket.connect(localSendIp, port);
	sf::Packet packet;
	packet << id;
	TCP_Socket.send(packet);
	TCP_Socket.setBlocking(false);

	//UDP
	UDP_Socket.bind(port);
	UDP_Socket.setBlocking(false);
	std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
	UDP_Socket.send(message.c_str(), message.size() + 1, publicSendIp, port);
}
