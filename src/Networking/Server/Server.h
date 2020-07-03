#ifndef NETWORK_SERVER
#define NETWORK_SERVER

#include "Networking/Client/Client.h"

class Server: public Network
{
private:
	Thread* tickThread;
protected:
	
public:
	//TCP
	sf::SocketSelector selector;
	sf::TcpListener TCP_listener;

	Server();
	virtual ~Server();
	
	virtual void UDP_send(sf::Packet &packet, sf::IpAddress &address);
	virtual void UDP_recieve(sf::Packet& packet, sf::IpAddress &address);
	virtual void TCP_send(sf::Packet &packet);
	virtual void TCP_recieve(sf::Packet&packet);

	sf::Time tickRate;

	void UDP_init();
	void TCP_init();
	void init();

	void connectClient();
	void disconnectClient();
	static void update(Server* server);
	static void run(Server* server);
};

#endif