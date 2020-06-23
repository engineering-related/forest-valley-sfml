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

	network->clock.restart().asMilliseconds();
	while (!network->quit)
	{
		if (network->clock.getElapsedTime().asMilliseconds() >= 10)
		{
			network->clock.restart().asMilliseconds();
			sf::Packet packet;
			network->UDP_Send(packet);
			network->UDP_Recieve(packet);
		}
	}
}

void Network::UDP_Send(sf::Packet &packet)
{
	//Send packet
	this->globalMutex.lock();
	if (prevPos != this->p1->rect.getPosition())
		packet << this->p1->rect.getPosition().x << this->p1->rect.getPosition().y;
	this->globalMutex.unlock();

	this->UDP_Socket.send(packet, this->sendIp, this->port);
}

void Network::UDP_Recieve(sf::Packet& packet)
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

void Network::TCP_Traffic(Network* network)
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

