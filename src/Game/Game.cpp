#include "Game.h"

Game::Game()
{
	this->init();
}

Game::~Game()
{
	delete this->window;
	delete this->font;
	delete this->map;
}

void Game::init()
{
	//Init Window
	util::Platform platform;
	this->window = new RenderWindow();
	float screenScalingFactor = platform.getScreenScalingFactor(this->window->getSystemHandle());
	VideoMode mode = VideoMode(WINDOW_WIDTH * screenScalingFactor, WINDOW_HEIGHT * screenScalingFactor);
	//VideoMode desktopMode = VideoMode().getDesktopMode();
	this->window->create(mode, "Forest Valley", Style::Default);
	srand(time(NULL));
	platform.setIcon(this->window->getSystemHandle());
	//window->setVerticalSyncEnabled(true);

	//Init Clock
	this->incer = 0.f;
	this->multiplier = MULTIPLIER;
	this->framerateLimit = FRAMERATELIMIT;
	this->cap = CAP;
	this->window->setFramerateLimit(this->framerateLimit);

	//Testing Map (TEMP)
	this->seed = MapGenerator::generatePsuedoRandomSeed();
	this->map = new MapGenerator(this->seed, Vector2i(200, 200), 30, 4, 0.5, 2, Vector2f(0, 0), 1);
	this->map->setDisplaySize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
}

void Game::pollEvents()
{
	Event event;
	while (this->window->pollEvent(event))
	{
		if (event.type == Event::Closed)
			this->window->close();
		if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.delta > 0)
				util::fn::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, *window, (1.f / 1.05));
			else if (event.mouseWheelScroll.delta < 0)
				util::fn::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, *window, 1.05);
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Escape))
		this->window->close();
}

void Game::updateClock()
{
	this->dt = this->clock.restart().asSeconds();
	this->FPS = 1.f / this->dt;

	if (this->incer >= this->cap)
	{
		this->incer = 0.f;
		//this->printFPS();
	}
	this->incer += this->dt * this->multiplier;
}

void Game::printFPS()
{
	std::cout << this->FPS << std::endl;
}

void Game::startLoop()
{
	while (this->window->isOpen())
	{
		//Events
		this->pollEvents();
		//Game clock
		this->updateClock();

		//Draw
		this->window->clear();
		this->map->draw(window);
		//this->map->update();
		this->window->display();
	}
}

void Game::start()
{
	this->startLoop();
}