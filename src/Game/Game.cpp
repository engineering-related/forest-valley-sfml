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
	delete this->world;
	delete this->camera;
}

void Game::init()
{
	//Init Window
	util::Platform platform;
	this->window = new RenderWindow();
	float screenScalingFactor = platform.getScreenScalingFactor(this->window->getSystemHandle());
	VideoMode mode = VideoMode(WINDOW_WIDTH * screenScalingFactor, WINDOW_HEIGHT * screenScalingFactor, 32);
	//VideoMode desktopMode = VideoMode().getDesktopMode();
	this->window->create(mode, "Forest Valley", Style::Default);
	this->onResize();
	srand(time(NULL));
	platform.setIcon(this->window->getSystemHandle());
	//window->setVerticalSyncEnabled(true);

	//Init Clock
	this->incer = 0.f;
	this->dt = 0.f;
	this->multiplier = MULTIPLIER;
	this->framerateLimit = FRAMERATELIMIT;
	this->cap = CAP;
	this->window->setFramerateLimit(this->framerateLimit);

	//Init World
	this->initWorld();

	//Init camera
	this->camera = new Camera(this->window);
	this->camera->setView(this->world->getPlayer()->getComponent<PositionComponent>().getCenterPosition(), this->window);
}

void Game::initWorld()
{
	this->seed = WorldGenerator::generatePsuedoRandomSeed();
	this->world = new World(this->seed);
}

void Game::handleInput()
{
	if (Keyboard::isKeyPressed(Keyboard::Escape))
		this->window->close();

	if (Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S))
	{
		this->world->getPlayer()->up(this->dt, this->multiplier);
	}
	if (Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::W))
	{
		this->world->getPlayer()->down(this->dt, this->multiplier);
	}
	if (Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D))
	{
		this->world->getPlayer()->left(this->dt, this->multiplier);
	}
	if (Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::A))
	{
		this->world->getPlayer()->right(this->dt, this->multiplier);
	}
}

void Game::onResize()
{
	sf::Vector2f size = static_cast<sf::Vector2f>(this->window->getSize());

	this->window->setSize(static_cast<sf::Vector2u>(size));

	sf::View newView(sf::FloatRect(0.f, 0.f, size.x, size.y));
	this->window->setView(newView);
}

void Game::pollEvents()
{
	Event event;
	while (this->window->pollEvent(event))
	{
		if (event.type == Event::Closed)
			this->window->close();
		if (event.type == Event::Resized)
			this->onResize();
		if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.delta > 0)
				Camera::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, *window, (1.f / 1.05));
			else if (event.mouseWheelScroll.delta < 0)
				Camera::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, *window, 1.05);
		}
	}
	this->handleInput();
}

void Game::updateClock()
{
	this->dt = this->clock.restart().asSeconds();
	this->FPS = 1.f / this->dt;

	if (this->incer >= this->cap)
	{
		this->incer = 0.f;
		this->printInfoToWindow();
	}
	this->incer += this->dt * this->multiplier;
}

void Game::printInfoToWindow()
{
	this->window->setTitle("Forest Valley | FPS: " + std::to_string(this->FPS) +
	" | SEED: " + std::to_string(this->seed));
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

		this->world->update(this->dt, this->multiplier);
		this->world->draw(this->window);

		//this->getPlayer()->getComponent<HitboxComponent>().draw(window);
		this->world->getMap()->draw(this->window);
		//this->camera->updateView(this->getPlayer()->getComponent<PositionComponent>().getCenterPosition(), this->window, this->dt, this->multiplier);
		this->camera->setView(this->world->getPlayer()->getComponent<PositionComponent>().getCenterPosition(), this->window);
		this->window->display();
	}
}

void Game::start()
{
	this->startLoop();
}