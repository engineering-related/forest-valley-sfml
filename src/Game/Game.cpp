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
	delete this->camera;
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
	this->dt = 0.f;
	this->multiplier = MULTIPLIER;
	this->framerateLimit = FRAMERATELIMIT;
	this->cap = CAP;
	this->window->setFramerateLimit(144);

	//Init Map
	this->initMap();

	//Testing Obejcts
	this->player = new Player(Vector2f(this->map->textureSize.x/2, this->map->textureSize.y/2));
	this->entites.push_back(this->player);

	//Init camera
	this->camera = new Camera(this->window);
	this->camera->setView(Vector2f(this->map->textureSize.x / 2, this->map->textureSize.y / 2), this->window);
}

void Game::initMap()
{
	this->map = new Map(&this->entites);
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
				Camera::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, *window, (1.f / 1.05));
			else if (event.mouseWheelScroll.delta < 0)
				Camera::zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, *window, 1.05);
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Escape))
		this->window->close();

	if (Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S))
	{
		this->player->up(this->dt, this->multiplier);
	}
	if (Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::W))
	{
		this->player->down(this->dt, this->multiplier);
	}
	if (Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D))
	{
		this->player->left(this->dt, this->multiplier);
	}
	if (Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::A))
	{
		this->player->right(this->dt, this->multiplier);
	}
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

		//Sort the Objects based on zIndex
		std::sort(this->entites.begin(), this->entites.end(), [](Object* obj1, Object* obj2) -> bool
		 {
			 if(obj1->getZIndex() == obj2->getZIndex())
			 {
				return obj1->getCenterPosition().x < obj2->getCenterPosition().x;
			 }
			else return obj1->getZIndex() < obj2->getZIndex();
		});

		//Update and draw Objects
		for(Object* object: this->entites)
		{
			object->update(this->dt, this->multiplier);
			object->draw(this->window);
		}
		this->map->map->draw(window);
		this->camera->updateView(this->player->getCenterPosition(), this->window, this->dt, this->multiplier);

		this->window->display();
		//Update view
	}
}

void Game::start()
{
	this->startLoop();
}