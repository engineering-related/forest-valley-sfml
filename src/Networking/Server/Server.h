#ifndef NETWORK_SERVER
#define NETWORK_SERVER

#include "Networking/Client/Client.h"

class Server: public Network
{
private:

protected:
	
public:
	//TCP
	sf::SocketSelector selector;
	sf::TcpListener TCP_listener;
	std::unordered_map<std::string, Client*> clients;

	Server();
	virtual ~Server();
	
	sf::Time tickRate;

	void UDP_init();
	void TCP_init();
	void init();

	void listenConnections();
	static void update(Server* server);
	static void run(Server* server);
};

#endif