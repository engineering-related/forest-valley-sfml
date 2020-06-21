#include "UDP_Client.h"


UDP_Client::UDP_Client(){

}

UDP_Client::~UDP_Client(){

}

void UDP_Client::connectToServer(){
	socket.bind(port);
	socket.setBlocking(false);

	sendIp = "78.72.205.138";
	std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
	socket.send(message.c_str(), message.size() + 1, sendIp, port);
}