#include "Network.h"

Network::Network()
{
	this->port = 24474;
	this->quit = false;

	this->p1 = new TestPlayer(sf::Color::Red, "Player 1");
	this->p2 = new TestPlayer(sf::Color::Blue, "Player 2");
}

Network::~Network()
{
	delete this->p1;
	delete this->p2;
}

void Network::UDP_Traffic(Network* network){
	static sf::Vector2f prevPos, p2Pos;
	static sf::Clock clock;
	clock.restart().asMilliseconds();
	while (!network->quit)
	{
		sf::Packet packet;
		if (clock.getElapsedTime().asMilliseconds() >= 10)
		{
			clock.restart().asMilliseconds();
			//Send packet
			network->globalMutex.lock();
			if (prevPos != network->p1->rect.getPosition())
				packet << network->p1->rect.getPosition().x << network->p1->rect.getPosition().y;

			network->globalMutex.unlock();

			network->UDP_Socket.send(packet, network->sendIp, network->port);
			//Receive packet
			network->UDP_Socket.receive(packet, network->sendIp, network->port);
			if (packet >> p2Pos.x >> p2Pos.y)
			{
				network->globalMutex.lock();
				network->p1->p2Pos = p2Pos;
				prevPos = network->p1->rect.getPosition();
				network->globalMutex.unlock();
			}
		}
	}
}


void Network::UDP_Run(){
	sf::Thread* thread = 0;
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Packets");
	window.setFramerateLimit(144);


	thread = new sf::Thread(&Network::UDP_Traffic, this);
	thread->launch();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				std::cout << "CLOSE" << std::endl;
				quit = true;
				window.close();
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

