#include "utils.hpp"

using namespace sf;

struct simMouse
{
	Vector2f pos = Vector2f(0, 0);
	Vector2f lastPos;

	bool left = false;
	bool right = false;
	bool push = false;
	bool isResting = false;
	float incer = 0;
	float cap = 200;


	void update(RenderWindow &window, const float &dt, const float &multiplier)
	{
		this->pos = window.mapPixelToCoords(Mouse::getPosition(window), window.getView());
		this->left = Mouse::isButtonPressed(Mouse::Button::Left);
		this->right = Mouse::isButtonPressed(Mouse::Button::Right);
		
	
		if (this->lastPos != this->pos)
		{
			this->incer = 0;
			this->isResting = false;
		}
		else
		{
			this->incer += dt * multiplier;
			if (this->incer >= this->cap)
			{
				this->incer = 0;
				this->isResting = true;
			}
		}
	}
		
	template <typename T>
	static bool rectColision(T obj, simMouse m)
	{
		if (obj.getPosition().x < m.pos.x + 1 &&
			obj.getPosition().x + obj.getGlobalBounds().width > m.pos.x &&
			obj.getPosition().y < m.pos.y + 1 &&
			obj.getPosition().y + obj.getGlobalBounds().height > m.pos.y) {
			return true;
		}
		else
		{
			return false;
		}
	}
};