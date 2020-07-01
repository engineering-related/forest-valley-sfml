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
	sf::UdpSocket testSocket;
	std::unordered_map<std::string, Client*> clients;

public:
	Server();
	virtual ~Server();
	
	sf::Time tickRate;

	void UDP_init();
	void TCP_init();
	void init();

	void listenConnections();
	void update();
	static void run(Server* server);
};

#endif