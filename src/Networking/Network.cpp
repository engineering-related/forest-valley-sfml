#include "Network.h"

sf::Mutex globalMutex;
sf::UdpSocket socket;
sf::IpAddress sendIp = "";
unsigned short port = 24474;
sf::Clock gameClock;
float dt;
bool quit = false;

struct TestPlayer
{
	sf::Vector2f velocity;
	sf::RectangleShape rect;
	float speed = 200.f;

	TestPlayer(sf::Color color)
	{
		rect.setSize(sf::Vector2f(20, 20));
		rect.setFillColor(color);
		this->velocity = sf::Vector2f(0.f, 0.f);
	}
	void update(const float& dt)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			velocity = sf::Vector2f(speed, 0.0f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			velocity = sf::Vector2f(-speed, 0.0f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			velocity = sf::Vector2f(0.0f, -speed);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			velocity = sf::Vector2f(0.0f, speed);
		else
			velocity = sf::Vector2f(0.0f, 0.0f);

		rect.move(velocity * dt);
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(rect);
	}
};

//Create two basic players
TestPlayer localPlayer(sf::Color::Red);
TestPlayer remotePlayer(sf::Color::Blue);

void handlePacketTraffic(void)
{
	static sf::Vector2f localPrevVel;
	static sf::Clock clock;
	clock.restart().asMilliseconds();
	while (!quit)
	{
		sf::Packet packet;
		//30ms delay for packets, avoid traffic overflow!
		if (clock.getElapsedTime().asMilliseconds() >= 30)
		{
			clock.restart().asMilliseconds();

			//Send packet
			globalMutex.lock();
			if (localPrevVel != localPlayer.velocity)
			{
				packet << localPlayer.velocity.x << localPlayer.velocity.y;
			}
			globalMutex.unlock();
			socket.send(packet, sendIp, port);

			//Receive packet
			socket.receive(packet, sendIp, port);
			if (packet >> remotePlayer.velocity.x >> remotePlayer.velocity.y)
			{
				remotePlayer.rect.move(remotePlayer.velocity);
				localPrevVel = localPlayer.velocity;
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
		sendIp = "78.72.205.138";
		std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
		socket.send(message.c_str(), message.size() + 1, sendIp, port);
	}

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Network Testing");

	socket.setBlocking(false);

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
		static float cap = 1;
		incer += dt;
		if (incer >= cap)
		{
			incer = 0;
			window.setTitle("SFML Network testing | FPS: " + std::to_string(FPS));
		}

		//globalMutex.lock();
		localPlayer.update(dt);
		localPlayer.draw(window);
		remotePlayer.draw(window);
		//globalMutex.unlock();

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
