#include "Network.h"

Network::Network()
{
	this->port = 24474;
	this->quit = false;

	this->p1 = new TestPlayer(sf::Color::Red, "Player 1");
	this->p2 = new TestPlayer(sf::Color::Blue, "Player 2");

	//UDP

	//TCP
}

Network::~Network()
{
	delete this->p1;
	delete this->p2;
}

void Network::UDP_traffic(Network* network){

	network->clock.restart().asMilliseconds();
	while (!network->quit)
	{
		if (network->clock.getElapsedTime().asMilliseconds() >= 10)
		{
			network->clock.restart().asMilliseconds();
			sf::Packet packet;
			network->UDP_send(packet);
			network->UDP_recieve(packet);
		}
	}
}

void Network::UDP_send(sf::Packet &packet)
{
	//Send packet
	this->globalMutex.lock();
	if (prevPos != this->p1->rect.getPosition())
		packet << this->p1->rect.getPosition().x << this->p1->rect.getPosition().y;
	this->globalMutex.unlock();

	this->UDP_Socket.send(packet, this->sendIp, this->port);
}

void Network::UDP_recieve(sf::Packet& packet)
{
	//Receive packet
	this->UDP_Socket.receive(packet, this->sendIp, this->port);
	if (packet >> this->p2Pos.x >> this->p2Pos.y)
	{
		this->globalMutex.lock();
		this->p1->p2Pos = this->p2Pos;
		this->prevPos = this->p1->rect.getPosition();
		this->globalMutex.unlock();
	}
}

void Network::TCP_traffic(Network* network)
{
	/*static sf::Vector2f prevPosition, p2Position;
	while(!quit)
	{
		sf::Packet packet;

		globalMutex.lock();
		if (prevPosition != rect1.getPosition())
			packet << rect1.getPosition().x << rect1.getPosition().y;
		globalMutex.unlock();

		socket.send(packet);

		socket.receive(packet);
		if(packet >> p2Position.x >> p2Position.y)
		{
			rect2.setPosition(p2Position);
			prevPosition = rect1.getPosition();
		}*/
}

void Network::UDP_run()
{
	sf::Thread* thread = 0;
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Packets");
	window.setFramerateLimit(144);


	thread = new sf::Thread(&Network::UDP_traffic, this);
	thread->launch();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				quit = true;
				window.close();
			}
			if(event.type == sf::Event::Resized)
			{
				sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());
				window.setSize(static_cast<sf::Vector2u>(size));
				sf::View newView(sf::FloatRect(0.f, 0.f, size.x, size.y));
				window.setView(newView);
			}
		}

		dt = gameClock.restart().asSeconds();
		float FPS = 1.f / dt;
		static float incer = 0;
		static float cap = 5;
		incer += dt;
		if (incer >= cap)
		{
			incer = 0;
			window.setTitle("Forest Valley | FPS: " + std::to_string(FPS));
		}

		globalMutex.lock();
		this->p1->draw(&window);
		this->p1->handleMouse(&window);
		this->p1->update(dt);
		this->p1->updatePlayers(dt, p2);
		this->p1->drawPlayers(&window, p2);
		globalMutex.unlock();

		window.display();
		window.clear();
	}
	if (thread)
	{
		thread->wait();
		delete thread;
	}
}
