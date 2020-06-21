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
		sf::Vector2f pos, endPos;
		sf::Vector2f velocity;
		bool mouseClicked;
		float speedMagnitude;

		TestPlayer(sf::Color color)
		{
			rect.setSize(sf::Vector2f(20, 20));
			rect.setFillColor(color);
			rect.setPosition(0.f, 0.f);
			this->speedMagnitude = 100.f;
			this->mouseClicked = false;
			this->pos = rect.getPosition();
			this->endPos = this->pos;
		}

		void draw(sf::RenderTarget* target)
		{
			target->draw(rect);
		}
		void update(const float &dt)
		{
			if(pos != endPos)
			{
				this->rect.move(this->velocity*dt);

			}
			else
			{
				this->velocity.x = 0;
				this->velocity.y = 0;
			}


			this->handleMouse();
		}
		void handleMouse()
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouseClicked)
			{
				mouseClicked = true;
				endPos = (sf::Vector2f)sf::Mouse::getPosition();
				float angle = atan2f(endPos.x - pos.x, endPos.y - pos.y);
				velocity = sf::Vector2f(speedMagnitude*cos(angle), speedMagnitude*sin(angle));
			}
			else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
				mouseClicked = false;
		}
	};
	TestPlayer* p1, *p2;
	UDP();
	virtual ~UDP();
	void run();
	static void handlePacketTraffic(UDP* UDP_Network);
};

#endif