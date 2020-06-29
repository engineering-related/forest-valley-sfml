#ifndef NETWORK_SERVER
#define NETWORK_SERVER

#include "Networking/Client/Client.h"

class Server: public Network
{
private:

protected:
	//UDP

	//TCP
	sf::SocketSelector selector;
	sf::TcpListener TCP_listener;

public:
	Server();
	virtual ~Server();
	
	void UDP_init();
	void TCP_init();
	void init();

	void listenConnections();
	void update();
	static void run(Server* server);
};

#endif