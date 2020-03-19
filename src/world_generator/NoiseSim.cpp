#include "utils.hpp"
#include "MapGenerator.h"
#include "Slider.hpp"

//https://www.redblobgames.com/maps/terrain-from-noise/

#define WIDTH 800
#define HEIGHT 800
using namespace sf;

void updateEvents(RenderWindow& window, Event &event)
{
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
		if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.delta > 0)
				//Utils::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / 1.05));
				return;
			else if (event.mouseWheelScroll.delta < 0)
				//Utils::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, 1.05);
				return;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Escape))
		window.close();
}

int main()
{

	//Init random
	srand(time(NULL));

	//Init clock
	Clock clock;
	float dt = 0;
	float multiplier = 144;

	//Init window
	VideoMode mode = VideoMode(WIDTH, HEIGHT);
	RenderWindow window(mode, "Map Generator", Style::Default);
	window.setFramerateLimit(multiplier);

	//Init font
	Font font;
	font.loadFromFile("Fonts/Prime-Regular.ttf");

	//Init map
	float isladSize = 0.1f;
	// A value that gives a nice distribution for 1024px bitmap
	const float scale_1024 = 0.0033f;
	// Fit the scale value for our bitmap size
	const float scale = (1024.0f / (float)isladSize) * scale_1024;

	MapGenerator map(time(NULL), Vector2i(100, 100), scale, 4, 0.5, 2, Vector2f(0, 0), 1);
	map.setDisplaySize(Vector2f(WIDTH, HEIGHT));

	std::vector<Slider<float>> floatSliders;
	std::vector<Slider<int>> intSliders;

	//Sliders init
	//Map generation
	Slider<int> s0(Vector2f(50.f, 50.f), &map.seed, Vector2f(0, INT_MAX), map.seed, &font, "Seed");
	intSliders.push_back(s0);
	Slider<float> s1(Vector2f(50.f, 100.f), &map.noiseScale, Vector2f(2, 100), map.noiseScale, &font, "Scale");
	floatSliders.push_back(s1);
	Slider<int> s2(Vector2f(50.f, 150.f), &map.octaves, Vector2f(1, 10), map.octaves, &font, "Octaves");
	intSliders.push_back(s2);
	Slider<float> s3(Vector2f(50.f, 200.f), &map.persistance, Vector2f(0, 1), map.persistance, &font, "Persistance");
	floatSliders.push_back(s3);
	Slider<float> s4(Vector2f(50.f, 250.f), &map.lacunarity, Vector2f(1, 10), map.lacunarity, &font, "Lacunarity");
	floatSliders.push_back(s4);
	Slider<float> s5(Vector2f(50.f, 300.f), &map.elevation, Vector2f(0.01, 10), map.elevation, &font, "Elevation");
	floatSliders.push_back(s5);

	//Color generation
	float height = 400.f;
	for (size_t i = 0; i < map.heightRegions.size(); i++)
	{
		Slider<float> s(Vector2f(50.f, height), &map.heightRegions[i].value, Vector2f(0.f, 1.f), map.heightRegions[i].value, &font, std::to_string(i));
		floatSliders.push_back(s);
		height += 50;
	}
	

	//Colors

	simMouse m;
	//Init simulation
	while (window.isOpen())
	{
		dt = clock.restart().asSeconds();
		Event event;
		updateEvents(window, event);
		if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
		{
			map.offset.x -= 0.1*dt*multiplier;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
		{
			map.offset.x += 0.1*dt*multiplier;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
		{
			map.offset.y -= 0.1*dt*multiplier;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
		{
			map.offset.y += 0.1*dt*multiplier;
		}

		//Drawing
		window.clear();
		map.draw(&window);
		m.update(window, dt, multiplier);
		map.update(dt, multiplier);

		if (!m.isResting || m.left)
		{
			for (auto &s : floatSliders)
			{
				s.draw<float>(window);
				s.update<float>(m);
			}
			for (auto &s : intSliders)
			{
				s.draw<int>(window);
				s.update<int>(m);
			}
		}
		
		window.display();
		m.lastPos = m.pos;
	}
}


