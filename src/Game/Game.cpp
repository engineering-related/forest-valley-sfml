#include "Game.h"
#include "Entity/AllEntities.hpp"

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
	this->window->create(mode, "Forest Valley", Style::None);
	srand(time(NULL));
	platform.setIcon(this->window->getSystemHandle());
	//window->setVerticalSyncEnabled(true);

	//Init Clock
	this->incer = 0.f;
	this->multiplier = MULTIPLIER;
	this->framerateLimit = FRAMERATELIMIT;
	this->cap = CAP;
	this->window->setFramerateLimit(this->framerateLimit);

	//Init Map
	this->initMap();

	//Testing Obejcts
}

void Game::initMap()
{
	this->map = new Map;
}

void Game::zoomViewAt(const sf::Vector2i& pixel, sf::RenderWindow& window, const float& zoom)
{
	const sf::Vector2f beforeCoord { window.mapPixelToCoords(pixel) };
	sf::View view { window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord { window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords { beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
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
				Game::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, *window, (1.f / 1.05));
			else if (event.mouseWheelScroll.delta < 0)
				Game::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, *window, 1.05);
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

		this->map->draw(this->window);

		for(Object* object: this->entites)
		{
			object->draw(this->window);
			object->update(this->dt, this->multiplier);
		}

		this->window->display();
	}
}

void Game::start()
{
	this->startLoop();
}