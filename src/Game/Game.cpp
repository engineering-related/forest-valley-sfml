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
	//Delete all entities
	for(Object* entity: this->entites)
	{
		delete entity;
	}
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

	//Init Map
	this->initMap();

	//Testing Obejcts
	this->player = new Player(Vector2f(this->map->pixelSize/2));
	this->entites.push_back(this->player);

	//Init camera
	this->camera = new Camera(this->window);
	this->camera->setView(this->player->getComponent<PositionComponent>().getCenterPosition(), this->window);
}

void Game::initMap()
{
	this->seed = MapGenerator::generatePsuedoRandomSeed();
	this->map = new Map(this->seed);
}

void Game::handleInput()
{
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

//Will be moved into a separate system in another file later
void Game::sortZindex()
{
	std::sort(this->entites.begin(), this->entites.end(), [](Object* obj1, Object* obj2) -> bool {
		if (obj1->getComponent<PositionComponent>().getZIndex() == obj2->getComponent<PositionComponent>().getZIndex())
		{
			if (obj1->getComponent<PositionComponent>().getCenterPosition().x == obj2->getComponent<PositionComponent>().getCenterPosition().x)
			{
				return obj1->getComponent<PositionComponent>().getCenterPosition().y > obj2->getComponent<PositionComponent>().getCenterPosition().y;
			}
			else
				return obj1->getComponent<PositionComponent>().getCenterPosition().x < obj2->getComponent<PositionComponent>().getCenterPosition().x;
		}
		else
			return obj1->getComponent<PositionComponent>().getZIndex() < obj2->getComponent<PositionComponent>().getZIndex();
	});
}

//Will be moved into a separate system in another file later
void Game::checkTileColision(Object* object, Chunk* chunk)
{
	if(object->hasComponent<MovementComponent>())
	{
		object->getComponent<PositionComponent>().setPosition(object->getComponent<MovementComponent>().getPrevPos());
		Vector2i topLeft = object->getComponent<PositionComponent>().getGridPos().topLeft;
		Vector2i bottomRight = object->getComponent<PositionComponent>().getGridPos().bottomRight;

		for (int x = topLeft.x - 1; x <= bottomRight.x + 1; x++)
		{
			for (int y = topLeft.y - 1; y <= bottomRight.y + 1; y++)
			{
				//Ensure bounds
				if(x < 0 || x > (int)chunk->grid.size() - 1 ||
		  		   y < 0 || y > (int)chunk->grid[0].size() - 1) return;
				//Check if tile is colidable
				else if (chunk->grid[x][y]->hasComponent<ColisionComponent>())
				{
					chunk->grid[x][y]->getComponent<ColisionComponent>().draw(window);
					IntRect r1 = object->getComponent<HitboxComponent>().getHitbox();
					IntRect r2 = chunk->grid[x][y]->getComponent<ColisionComponent>().getColisionIntRect();
					IntRect nextFrame = r1;
					Vector2f vel = MovementComponent::getClampedMagVel(object->getComponent<MovementComponent>().getVel())*dt;

					//Nextframe vel needs to be a rounded to the closest int in the direction of the wall
					if(vel.x < 0.f) vel.x = floor(vel.x);
					if(vel.x > 0.f) vel.x = ceil(vel.x);
					if(vel.y < 0.f) vel.y = floor(vel.y);
					if(vel.y > 0.f) vel.y = ceil(vel.y);

					//Calc the position of the next frame
					nextFrame.left += vel.x;
					nextFrame.top += vel.y;

					if(r2.intersects(nextFrame))
					{
						//Bottom colision
						if(r1.top < r2.top &&
						   r1.top + r1.height < r2.top + r2.height &&
						   r1.left < r2.left + r2.width &&
						   r1.left + r1.width > r2.left)
						{
							object->getComponent<MovementComponent>().setVel(Vector2f(object->getComponent<MovementComponent>().getVel().x, 0.f));
							object->getComponent<PositionComponent>().setPosition(Vector2f(r1.left, r2.top - r1.height));
						}
						//Top colision
						else if(r1.top > r2.top &&
						   r1.top + r1.height > r2.top + r2.height &&
						   r1.left < r2.left + r2.width &&
						   r1.left + r1.width > r2.left)
						{
							object->getComponent<MovementComponent>().setVel(Vector2f(object->getComponent<MovementComponent>().getVel().x, 0.f));
							object->getComponent<PositionComponent>().setPosition(Vector2f(r1.left, r2.top + r2.height));
						}

						//Right colision
						if(r1.left < r2.left &&
						   r1.left + r1.width < r2.left + r2.width &&
						   r1.top < r2.top + r2.height &&
						   r1.top + r1.height > r2.top)
						{
							object->getComponent<MovementComponent>().setVel(Vector2f(0.f, object->getComponent<MovementComponent>().getVel().y));
							object->getComponent<PositionComponent>().setPosition(Vector2f(r2.left - r1.width, r1.top));
						}
						//Left colision
						else if(r1.left > r2.left &&
						   r1.left + r1.width > r2.left + r2.width &&
						   r1.top < r2.top + r2.height &&
						   r1.top + r1.height > r2.top)
						{
							object->getComponent<MovementComponent>().setVel(Vector2f(0.f, object->getComponent<MovementComponent>().getVel().y));
							object->getComponent<PositionComponent>().setPosition(Vector2f(r2.left + r2.width, r1.top));
						}
					}
				}
				else continue;
			}
		}
		object->getComponent<MovementComponent>().moveSprite(object->getComponent<MovementComponent>().getVel(), dt);
	}
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

		//Testing with rendering chunks, should use a threadpool later!
		Vector2i playerChunkPos = this->player->getComponent<PositionComponent>().getChunkPos();

		for(int x = playerChunkPos.x - 1; x <= playerChunkPos.x + 1; x++)
		{
			for(int y = playerChunkPos.y - 1; y <= playerChunkPos.y + 1; y++)
			{
				if(x >= 0 && x < map->chunkAmount.x &&
				   y >= 0 && y < map->chunkAmount.y)
				{
					if(!this->map->chunks[x][y]->loaded)
					{
						this->map->chunks[x][y]->load();
						this->map->map->updateTexture(this->map->chunks[x][y]->gridPos, this->map->chunks[x][y]->terrainVec);
					}
					this->map->chunks[x][y]->drawTiles(this->window);
				}
			}
		}

		//Testing with rendering chunks, should use a threadpool later!
		static Vector2i oldPlayerChunksPos;
		if(oldPlayerChunksPos != playerChunkPos)
		{
			this->entites.clear();
			for(int x = playerChunkPos.x - 1; x <= playerChunkPos.x + 1; x++)
			{
				for(int y = playerChunkPos.y - 1; y <= playerChunkPos.y + 1; y++)
				{
					if(x >= 0 && x < map->chunkAmount.x &&
						y >= 0 && y < map->chunkAmount.y)
					{
						this->entites.insert(this->entites.end(), this->map->chunks[x][y]->dynamicEntities.begin(), this->map->chunks[x][y]->dynamicEntities.end());
					}
				}
			}
			this->entites.push_back(this->player);
		}
		oldPlayerChunksPos = playerChunkPos;

		//Sort the Objects based on zIndex
		this->sortZindex();

		//Update and draw Objects
		for(Object* object: this->entites)
		{
			object->update(this->dt, this->multiplier);
			//this->checkTileColision(object, this->map->chunks[playerChunkPos.x][playerChunkPos.y]);
			object->draw(this->window);
		}
		//this->player->getComponent<HitboxComponent>().draw(window);
		this->map->map->draw(window);
		//this->camera->updateView(this->player->getComponent<PositionComponent>().getCenterPosition(), this->window, this->dt, this->multiplier);
		this->camera->setView(player->getComponent<PositionComponent>().getCenterPosition(), this->window);
		this->window->display();
	}
}

void Game::start()
{
	this->startLoop();
}