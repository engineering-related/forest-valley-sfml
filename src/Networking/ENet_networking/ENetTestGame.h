#ifndef ENET_TEST_GAME
#define ENET_TEST_GAME

#include "ENetTestPlayer.h"

class ENetwork;

class ENetTestGame
{
private:
	const int maxNrSavedGameStates = 5;
	bool gameLoopRunning;

public:
	ENetTestGame(const ENetwork * const context);
	~ENetTestGame();

	//Game States
	/////////////////////////////////////////////////////////////////
	struct State
	{
		int gameStateID;
		int64_t timeStamp;
		std::unordered_map<int /*playerStateID*/, ENetTestPlayer::State /*playerState*/> playerStates;

		//WARNING: Need more info about the map and saves etc.

		//Constructor
		State(ENetTestGame* context = NULL);

	}; State gameState, lastGameState;

	std::vector<State> gameStates;

	//WARNING: Returns the current changed states of the game
	const char* getChangedStateData(const std::string& ENetID,  const unsigned int &packetType);
	void setChangedStateData(const std::vector<std::string> &gameStateDataVec);

	void refreshState();

	//WARNING: Returns the CURRENT frame/snapshot of the game
	const char* getGameData(const std::string& ENetID,  const unsigned int &packetType);
	void setGameData(const std::vector<std::string> &gameDataVec);
	/////////////////////////////////////////////////////////////////

	//Attributes
	const ENetwork * context;
	float dt;
	sf::RenderWindow* window;
	std::unordered_map<std::string /*ENetID*/, ENetTestPlayer* /*Players*/> players;
	sf::Clock clock;
	const std::string* ENetID;

	//Network and game mutex-lock
	pthread_mutex_t ENetMutex;

	//Getters
	inline const bool& getGameLoopRunning()const{return gameLoopRunning;}

	//Setters
	inline void setGameLoopRunning(const bool &state){gameLoopRunning = state;}

	//Functions
	void initTestPlayer(const std::string *ENetID);
	void updatePlayers(const float &dt);
	void drawPlayers(sf::RenderTarget* target);
	void addPlayer(std::string ENetID, ENetTestPlayer* player);
	void removePlayer(const std::string &ENetID);
	void setPlayerData(const char* playerData);
	void draw(sf::RenderTarget* target);
	void update(const float& dt);
	void loop();
};


#endif //ENET_TEST_GAME