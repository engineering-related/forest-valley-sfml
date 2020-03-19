#include "simMouse.hpp"

template <typename T>
class Slider
{
private:
	Vector2f smallRectPos;
	Vector2f bigRectPos;
	Vector2f rect;
	RectangleShape bigRect;
	RectangleShape smallRect;
	bool adjusting = false;
	T *value;
	float copyValue;
	T startValue;
	Vector2f interval;
	std::string name;
	Text text;
	float padding;

public:
	Slider(Vector2f pos, T *value, Vector2f interval, T startValue, Font *font, std::string name)
	{
		this->bigRectPos = pos;
		this->rect = Vector2f(200, 20);
		this->padding = 20;
		
		//Sliders
		this->bigRect.setFillColor(Color(100, 100, 100));
		this->smallRect.setFillColor(Color(200, 200, 200));
		this->interval = interval;
		this->value = value;
		this->copyValue = *value;
		this->startValue = startValue;
		//Text
		this->text.setFont(*font);
		this->text.setString(std::to_string(*this->value));
		this->text.setCharacterSize(17);
		this->name = name;

		this->smallRectPos = Vector2f(Utils::map(startValue, interval.x, interval.y, pos.x, pos.x + this->rect.x - this->rect.y), pos.y);
	}
	~Slider()
	{

	}

	template <typename T>
	void draw(RenderWindow &window)
	{
		//Slider
		this->bigRect.setPosition(Vector2f(window.mapPixelToCoords(Vector2i(this->bigRectPos), window.getView())));
		this->smallRect.setPosition(Vector2f(window.mapPixelToCoords(Vector2i(this->smallRectPos), window.getView())));

		Vector2f scaledRect = Vector2f(this->rect.x / window.getDefaultView().getSize().x * window.getView().getSize().x,
			this->rect.y / window.getDefaultView().getSize().y * window.getView().getSize().y);

		this->bigRect.setSize(scaledRect);
		this->smallRect.setSize(Vector2f(scaledRect.y, scaledRect.y));

		window.draw(this->bigRect);
		window.draw(this->smallRect);

		//Text
		this->text.setPosition(this->bigRect.getPosition());
		this->text.setScale(window.getView().getSize().x / window.getDefaultView().getSize().x, window.getView().getSize().y / window.getDefaultView().getSize().y);
		window.draw(this->text);
		this->drawText<T>(window);
	}

	template <typename T>
	void drawText(RenderWindow &window)
	{
		Vector2f prevTextPos = this->text.getPosition();
		this->text.setPosition(Vector2f(this->bigRect.getPosition().x + this->bigRect.getGlobalBounds().width + this->bigRect.getGlobalBounds().width/10, this->bigRect.getPosition().y));
		
		std::string prevString = this->text.getString();
		this->text.setString(this->name);
	
		window.draw(this->text);
		
		this->text.setPosition(prevTextPos);
		this->text.setString(prevString);
	}

	template <typename T>
	void update(simMouse m)
	{
		if (simMouse::rectColision(smallRect, m) && m.left && !adjusting)
		{
			adjusting = true;
		}
		if (adjusting)
		{
			this->smallRectPos.x = m.pos.x - 15;
			if (this->smallRectPos.x < this->bigRectPos.x) this->smallRectPos.x = this->bigRectPos.x;
			if (this->smallRectPos.x + this->rect.y > this->bigRectPos.x + this->rect.x) this->smallRectPos.x = this->bigRectPos.x + this->rect.x - this->rect.y;
		}
		if (!m.left)
		{
			adjusting = false;
		}

		//Value
		*this->value = Utils::map(this->smallRectPos.x, this->bigRectPos.x, this->bigRectPos.x + this->rect.x - this->rect.y, this->interval.x, this->interval.y);
		
		this->text.setString(Utils::round2D(*this->value));

	}
};