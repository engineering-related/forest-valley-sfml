#ifndef NETWORK_UDP
#define NETWORK_UDP

#include "Networking/Network.h"

class UDP: public Network
{
private:

protected:
	sf::UdpSocket socket;

public:
	struct TestPlayer
	{
		sf::RectangleShape rect;
		sf::Vector2f endPos;
		sf::Vector2f velocity;
		bool mouseClicked;
		float speedMagnitude;

		TestPlayer(sf::Color color)
		{
			rect.setSize(sf::Vector2f(20, 20));
			rect.setFillColor(color);
			rect.setPosition(0.f, 0.f);
			rect.setOrigin(rect.getSize()*0.5f);
			this->speedMagnitude = 200.f;
			this->mouseClicked = false;
			this->endPos = this->rect.getPosition();
			this->velocity = sf::Vector2f(0.f, 0.f);
		}

		void draw(sf::RenderTarget* target)
		{
			target->draw(rect);
		}
		void update(const float &dt)
		{
			if(sf::Vector2i(round(this->rect.getPosition().x), round(this->rect.getPosition().y)) != sf::Vector2i(round(endPos.x), round(endPos.y)))
			{
				this->rect.move(this->velocity*dt);
			}
			else
			{
				this->velocity.x = 0;
				this->velocity.y = 0;
			}
		}
		void handleMouse(sf::RenderWindow* window)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window->getViewport(window->getView()).contains(mousePos))
			{
				mouseClicked = true;
				endPos = (sf::Vector2f)mousePos;
				float angle = atan2f(endPos.y - this->rect.getPosition().y, endPos.x - this->rect.getPosition().x);
				velocity = sf::Vector2f(speedMagnitude*cos(angle), speedMagnitude*sin(angle));
			}
			else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
				mouseClicked = false;
		}
	};
	TestPlayer* p1;
	TestPlayer *p2;
	UDP();
	virtual ~UDP();
	void run();
	static void handlePacketTraffic(UDP* UDP_Network);
};

#endif