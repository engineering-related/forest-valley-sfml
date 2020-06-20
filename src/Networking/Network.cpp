#include "Network.h"

sf::Mutex globalMutex;
sf::RectangleShape rect1, rect2;
sf::UdpSocket socket;
sf::IpAddress sendIp = "";
unsigned short port = 24474;
bool quit = false;
float dt;
sf::Clock gameClock;

void handlePacketTraffic(void)
{
	static sf::Vector2f prevPosition, p2Position;
	static sf::Clock clock;
	clock.restart().asMilliseconds();
	while (!quit)
	{
		sf::Packet packet;
		if (clock.getElapsedTime().asMilliseconds() >= 30)
		{
			clock.restart().asMilliseconds();
			//Send packet
			globalMutex.lock();
			if (prevPosition != rect1.getPosition())
				packet << rect1.getPosition().x << rect1.getPosition().y;
			globalMutex.unlock();

			socket.send(packet, sendIp, port);
			//Receive packet
			socket.receive(packet, sendIp, port);
			if (packet >> p2Position.x >> p2Position.y)
			{
				globalMutex.lock();

				rect2.setPosition(p2Position);
				prevPosition = rect1.getPosition();
				globalMutex.unlock();
			}
		}
	}
}

int main()
{
	sf::Thread* thread = 0;

	char connectionType;

	std::cout << "s)erver or c)lient" << std::endl;
	std::cin >> connectionType;

	//std::map<unsigned short, sf:IpAddress> computerID;

	socket.bind(port);
	socket.setBlocking(false);

	if (connectionType == 's')
	{
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
				std::cout << sender.toString() << " said: " << buffer << std::endl;
				receviedIp = true;
				sendIp = sender;
			}
		}
	}
	else
	{
		sendIp = "192.168.72.118";//<-- LP "78.72.205.138";
		std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
		socket.send(message.c_str(), message.size() + 1, sendIp, port);
	}

	rect1.setSize(sf::Vector2f(20, 20));
	rect2.setSize(sf::Vector2f(20, 20));

	rect1.setFillColor(sf::Color::Red);
	rect2.setFillColor(sf::Color::Blue);

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Packets");

	//socket.setBlocking(false);

	thread = new sf::Thread(&handlePacketTraffic);
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
	return 0;
}
