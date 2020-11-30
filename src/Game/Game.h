#ifndef GAME
#define GAME

//Includes
#include "Camera/Camera.h"
#include "World/World.h"
#include "Platform/Platform.hpp"
#include "Networking/Networking.hpp"

using namespace sf;

class Game
{
private:
	RenderWindow* window;
	Camera* camera;
	Clock clock;
	Font* font;
	bool gameLoopRunning;

	float FPS;
	float dt;
	float incer;
	float multiplier;
	float framerateLimit;
	float cap;

	unsigned int seed;
	World* world;

	//Game States
	const int maxNrSavedGameStates = 5;

	struct State
	{
		inline static sf::Uint32 totalStates;
		sf::Uint32 ID;
		int64_t timeStamp;
		std::unordered_map<sf::Uint16  /*ENetID*/, ENetTestPlayer::State /*playerState*/> playerStates;

		//WARNING: Need more info about the map and saves etc.

		State(ENetTestGame* context = NULL);

	}; //State gameState, lastGameState;

	std::vector<State> gameStates;

	void init();
	void initWorld();
	void handleInput();
	void onResize();
	void pollEvents();
	void updateClock();
	void printInfoToWindow();

	void startLoop();

public:
	Game();
	~Game();

	//WARNING: Returns the current changed states of the game
	sf::Packet getChangedStateData(const sf::Uint16 & ENetID, const unsigned int &packetType);
	void setChangedStateData(sf::Packet& packet);

	void refreshState();

	//WARNING: Returns the CURRENT frame/snapshot of the game
	sf::Packet getGameData(const sf::Uint16 & ENetID,  const unsigned int &packetType);
	void setGameData(sf::Packet& packet);

	//Attributes
	const ENetwork * context;
	const sf::Uint16* ENetID_ptr;

	//Network and game mutex-lock
	pthread_mutex_t ENetMutex;

	//Getters
	inline const bool& getGameLoopRunning()const{return gameLoopRunning;}

	//Setters
	inline void setGameLoopRunning(const bool &state){gameLoopRunning = state;}

	//Functions
	void start();
};

#endif
