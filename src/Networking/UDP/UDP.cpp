#include "UDP.h"

UDP::UDP()
{

}

UDP::~UDP()
{

}


void UDP::handlePacketTraffic(UDP* UDP_Network)
{
	static sf::Vector2f prevPosition, p2Position;
	static sf::Clock clock;
	clock.restart().asMilliseconds();
	while (!UDP_Network->quit)
	{
		sf::Packet packet;
		if (clock.getElapsedTime().asMilliseconds() >= 20)
		{
			clock.restart().asMilliseconds();
			//Send packet
			UDP_Network->globalMutex.lock();
			if (prevPosition != UDP_Network->rect1.getPosition())
				packet << UDP_Network->rect1.getPosition().x << UDP_Network->rect1.getPosition().y;
			UDP_Network->globalMutex.unlock();

			UDP_Network->socket.send(packet, UDP_Network->sendIp, UDP_Network->port);
			//Receive packet
			UDP_Network->socket.receive(packet, UDP_Network->sendIp, UDP_Network->port);
			if (packet >> p2Position.x >> p2Position.y)
			{
				UDP_Network->globalMutex.lock();

				UDP_Network->rect2.setPosition(p2Position);
				prevPosition = UDP_Network->rect1.getPosition();
				UDP_Network->globalMutex.unlock();
			}
		}
	}
}
