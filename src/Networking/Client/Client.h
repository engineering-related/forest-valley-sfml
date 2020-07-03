#ifndef NETWORK_CLIENT
#define NETWORK_CLIENT

#include "Networking/Network.h"

class Client: public Network
{
	private:
		/* data */
	protected:
		//UDP

		//TCPz

	public:
		Client();
		virtual ~Client();
		static void traffic(Client* client);

		void UDP_send(sf::Packet &packet, sf::IpAddress &address);
		void UDP_recieve(sf::Packet& packet, sf::IpAddress &address);
		void TCP_send(sf::Packet &packet);
		void TCP_recieve(sf::Packet&packet);

		void TCP_connect();
		void UDP_connect();
		void addPlayer(sf::Packet &packet);
		void removePlayer(sf::Packet &packet);
		void connectToServer();
		void disconnectFromServer();
};

#endif