#include "Client.h"

Client::Client(/* args */)
{

}

Client::~Client()
{

}


void Client::connectToServer(){
	UDP_Socket.bind(port);
	UDP_Socket.setBlocking(false);

	sendIp = "78.72.205.138";
	std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
	UDP_Socket.send(message.c_str(), message.size() + 1, sendIp, port);
}
