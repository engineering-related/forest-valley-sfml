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

	Server();
	virtual ~Server();
	
	void UDP_send(Network* n, sf::Packet &packet, sf::IpAddress &address);
	void UDP_recieve(sf::Packet& packet, sf::IpAddress &address);
	void TCP_send(Network* n, sf::Packet &packet);
	void TCP_recieve(sf::Packet&packet);

	sf::Time tickRate;

	void UDP_init();
	void TCP_init();
	void init();
	void done();

	void checkNewClientConnection();
	void disconnectClient(Network* client);
	static void update(Server* server);
	static void run(Server* server);
};

#endif