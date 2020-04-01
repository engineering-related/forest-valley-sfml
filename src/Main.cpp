#include "Platform/Platform.hpp"
#include "MapGenerator/MapGenerator.h"


//CTRL + SHIFT + B TO BUILD AND RUN

#define WIDTH 800
#define HEIGHT 800

using namespace sf;

/*void updateEvents(RenderWindow& window, Event& event)
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
}*/

int main()
{
	util::Platform platform;

#if defined(_DEBUG)
	std::cout << "DEBUGGING" << std::endl;
#endif
	//Start of program here:

	//Init random
	srand(time(NULL));

	//Init clock
	Clock clock;
	//float dt = 0;
	//float multiplier = 144;

	sf::RenderWindow window;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	window.create(sf::VideoMode(WIDTH * screenScalingFactor, HEIGHT * screenScalingFactor), "Noise Simulation");
	//platform.setIcon(window.getSystemHandle());

	unsigned int seed = MapGenerator::generatePsuedoRandomSeed();

	MapGenerator map(seed, Vector2i(200, 200), 30, 4, 0.5, 2, Vector2f(0, 0), 1);
	map.setDisplaySize(Vector2f(WIDTH, HEIGHT));

	PerlinNoise noise();

	while (window.isOpen())
	{
		//dt = clock.restart().asSeconds();
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.delta > 0)
					//Utils::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / 1.05));
					break;
				else if (event.mouseWheelScroll.delta < 0)
					//Utils::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, 1.05);
					break;
			}
		}

		window.clear();
		map.draw(&window);
		map.update();
		window.display();
	}

	return 0;
}
