#ifndef NETWORK_SERVER
#define NETWORK_SERVER

#include "Networking/Network.h"

class Server: public Network
{
private:
	
protected:
	//UDP

	//TCP
	sf::SocketSelector selector;
	sf::TcpListener TCP_listener;
	std::vector<sf::TcpSocket*> clients;

public:
	Server(/* args */);
	virtual ~Server();
	
	void UDP_init();
	void TCP_init();
	void init();
	
};

#endif