#include "UDP_Server.h"

UDP_Server::UDP_Server()
{

}

UDP_Server::~UDP_Server()
{

}


void UDP_Server::init()
{
	sf::Thread* thread = 0;

	char connectionType;

	socket.bind(port);
	socket.setBlocking(false);

	bool receviedIp = false;
	while (!receviedIp)
	{
		// Receive a message from anyone
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);
		if (received > 0)
		{
			std::cout << received << std::endl;
			std::cout << sender.toString() << " connected with " << buffer << std::endl;
			receviedIp = true;
			sendIp = sender;
		}
	}

	rect1.setSize(sf::Vector2f(20, 20));
	rect2.setSize(sf::Vector2f(20, 20));

	rect1.setFillColor(sf::Color::Red);
	rect2.setFillColor(sf::Color::Blue);

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Packets");

	//socket.setBlocking(false);

	thread = new sf::Thread(&UDP::handlePacketTraffic, this);
	thread->launch();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
			{
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			rect1.move(300.f*dt, 0.0f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			rect1.move(-300.f*dt, 0.0f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			rect1.move(0.0f, -300.f*dt);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			rect1.move(0.0f, 300.f*dt);

		window.draw(rect1);
		window.draw(rect2);
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