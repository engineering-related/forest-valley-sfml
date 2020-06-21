#include "UDP.h"

UDP::UDP()
{
	this->p1 = new TestPlayer(sf::Color::Red);
	this->p2 = new TestPlayer(sf::Color::Blue);
}

UDP::~UDP()
{
	delete this->p1;
	delete this->p2;
}


void UDP::handlePacketTraffic(UDP* UDP_Network)
{
	static sf::Vector2f prevEndPos, p2EndPos, p2Vel;
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
			if (prevEndPos != UDP_Network->p1->endPos)
			{
				packet << UDP_Network->p1->endPos.x << UDP_Network->p1->endPos.y <<
						  UDP_Network->p1->velocity.x << UDP_Network->p1->velocity.y;
			}
			UDP_Network->globalMutex.unlock();

			UDP_Network->socket.send(packet, UDP_Network->sendIp, UDP_Network->port);
			//Receive packet
			UDP_Network->socket.receive(packet, UDP_Network->sendIp, UDP_Network->port);
			if (packet >> p2EndPos.x >> p2EndPos.y >> p2Vel.x >> p2Vel.y)
			{
				UDP_Network->globalMutex.lock();

				UDP_Network->p2->endPos = p2EndPos;
				UDP_Network->p2->velocity = p2Vel;
				prevEndPos = UDP_Network->p1->endPos;
				UDP_Network->globalMutex.unlock();
			}
		}
	}
}


void UDP::run(){
	sf::Thread* thread = 0;
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Packets");
	window.setFramerateLimit(144);

	//socket.setBlocking(false);

	thread = new sf::Thread(&UDP::handlePacketTraffic, this);
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
		this->p2->draw(&window);
		this->p1->handleMouse(&window);
		this->p1->update(dt);
		this->p2->update(dt);
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
