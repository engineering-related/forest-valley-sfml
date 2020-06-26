#include "Client.h"

Client::Client(/* args */)
{
	this->sendIp =  "87.96.222.250"; //"78.72.205.138";
	std::cout << sf::IpAddress::getPublicAddress().toString() << std::endl;
	std::cout << sf::IpAddress::getLocalAddress().toString() << std::endl;
}

Client::~Client()
{

}

void Client::connectToServer(){
	UDP_Socket.bind(port);
	UDP_Socket.setBlocking(false);
	std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
	UDP_Socket.send(message.c_str(), message.size() + 1, sendIp, port);
}
