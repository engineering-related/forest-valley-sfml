#ifndef ENET_TEST_GAME
#define ENET_TEST_GAME

#include "ENetTestPlayer.h"

class ENetwork;

class ENetTestGame
{
private:
	bool gameLoopRunning;

public:
	ENetTestGame(const ENetwork * const context);
	~ENetTestGame();

	const ENetwork * context;
	float dt;
	sf::RenderWindow* window;
	std::unordered_map<std::string, ENetTestPlayer*> players;
	sf::Clock clock;
	std::string* ENetID;

	//Network and game mutex-lock
	pthread_mutex_t ENetMutex;

	//Getters
	inline const bool& getGameLoopRunning()const{return gameLoopRunning;}

	//Setters
	inline void setGameLoopRunning(const bool &state){gameLoopRunning = state;}

	//Functions
	void initTestPlayer(std::string *ENetID);
	void updatePlayers(const float &dt);
	void drawPlayers(sf::RenderTarget* target);
	void loop();
};


#endif //ENET_TEST_GAME