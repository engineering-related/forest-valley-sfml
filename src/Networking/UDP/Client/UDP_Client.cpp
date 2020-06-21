#include "UDP_Client.h"


UDP_Client::UDP_Client(){

}

UDP_Client::~UDP_Client(){

}

void UDP_Client::connectToServer(){
	sf::Thread* thread = 0;

	socket.bind(port);
	socket.setBlocking(false);

	sendIp = "78.72.205.138";
	std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
	socket.send(message.c_str(), message.size() + 1, sendIp, port);

	rect1.setSize(sf::Vector2f(20, 20));
	rect2.setSize(sf::Vector2f(20, 20));

	rect1.setFillColor(sf::Color::Red);
	rect2.setFillColor(sf::Color::Blue);

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Packets");

	thread = new sf::Thread(&handlePacketTraffic, this);
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