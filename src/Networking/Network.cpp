#include "Network.h"

Network::Network()
{
	this->port = 24474;
	this->quit = false;
	this->UDP_send_packet = true;
	this->TCP_send_packet = true;
	this->localSendIp = "";
	this->publicSendIp = "";
	this->delay = 20;

	//Player creation
	srand(time(NULL));
	sf::Color playerColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
	this->player = new TestPlayer(playerColor);
}

Network::~Network()
{
	for (auto p : players)
		delete p.second;
	delete player;
}

void Network::updatePlayers(const float &dt)
{
	for (auto p : players)
	{
		sf::Vector2f d(p.second->player->p2Pos.x - p.second->player->rect.getPosition().x,
					   p.second->player->p2Pos.y - p.second->player->rect.getPosition().y);
		if(abs(d.x) >= 1 && abs(d.y) >= 1)
		{
			float angle = atan2f(d.y, d.x);
			p.second->player->velocity = sf::Vector2f(p.second->player->speedMagnitude * cos(angle),
													  p.second->player->speedMagnitude * sin(angle));
			p.second->player->rect.move(p.second->player->velocity * dt);
		}
	}
	this->player->update(dt);
}

void Network::drawPlayers(sf::RenderTarget* target)
{
	for (auto p : players)
	{
		p.second->player->draw(target);
	}

	this->player->draw(target);
}

void Network::traffic(Network* network)
{
	network->clock.restart().asMilliseconds();
	while (!network->quit)
	{
		if (network->clock.getElapsedTime().asMilliseconds() >= network->delay)
		{
			network->clock.restart().asMilliseconds();
			network->UDP_send_packet = true;
		}

		sf::Packet UDP_packet, TCP_packet;
		if(network->UDP_send_packet)
		{
			//network->UDP_send(UDP_packet);
			network->UDP_send_packet = false;
		}
		network->globalMutex.lock();
		if(network->player->mouseClicked)
		{
			//Send TCP data later
			network->TCP_send(TCP_packet);
			network->TCP_send_packet = false;
		}
		network->globalMutex.unlock();

		network->UDP_recieve(UDP_packet, true);
		network->TCP_recieve(TCP_packet);
	}
}

void Network::UDP_send(sf::Packet &packet)
{
	//Send packet
	this->globalMutex.lock();
	if (this->player->prevPos != this->player->rect.getPosition())
		packet << this->player->rect.getPosition().x << this->player->rect.getPosition().y;
	this->globalMutex.unlock();

	this->UDP_Socket.send(packet, this->localSendIp, this->port);
}

void Network::UDP_recieve(sf::Packet& packet, bool empty)
{
	//Receive packet
	if(empty)
		this->UDP_Socket.receive(packet, this->localSendIp, this->port);
	std::string id;
	Vector2f pos;
	if (packet >> id >> pos.x >> pos.y)
	{
		this->globalMutex.lock();
		players[id]->player->p2Pos = pos;
		this->player->prevPos = this->player->rect.getPosition();
		this->globalMutex.unlock();
	}
}

void Network::TCP_send(sf::Packet &packet)
{
	this->globalMutex.lock();
	packet << this->player->endPos.x << this->player->endPos.y;
	this->globalMutex.unlock();
	this->TCP_Socket.send(packet);
}

void Network::addPlayer(sf::Packet &packet)
{
	std::string id;
	std::string cLocalIp;
	sf::Vector2f pos;
	sf::Int32 r, g, b;
	packet >> id >> cLocalIp >>
			  pos.x >> pos.y >>
	          r >> g >> b;

	Network* p = new Network();
	sf::Color color(r, g, b);
	p->id = id;
	p->localIp = sf::IpAddress(cLocalIp);
	p->player->rect.setPosition(pos);
	p->player->rect.setFillColor(color);
	this->globalMutex.lock();
	this->players[id] = p;
	this->globalMutex.unlock();
}

void Network::removePlayer(sf::Packet &packet)
{

}

void Network::TCP_recieve(sf::Packet &packet)
{
	int type;
	packet >> type;
	switch ((TCP_type)type)
	{
	case TCP_type::PLAYER_CONNECTED:
		addPlayer(packet);
		break;
	case TCP_type::PLAYER_LEFT:
		UDP_recieve(packet, false);
		break;
	case TCP_type::SERVER_QUIT:
		quit = true;
		break;
	case TCP_type::GAME_PAUSED:
		//Pause the game...
		break;
	default:
		break;
	}
}

void Network::start()
{
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Packets");
	window.setFramerateLimit(144);
	this->thread->launch();
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
		if(quit)
			window.close();

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
		this->player->handleMouse(&window);
		this->updatePlayers(dt);
		this->drawPlayers(&window);
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
