#ifndef ENET_TEST_GAME
#define ENET_TEST_GAME

#include "Game/Camera/Camera.h"
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
		inline static sf::Uint32 totalStates;
		sf::Uint32 ID;
		int64_t timeStamp;
		std::unordered_map<sf::Uint16  /*ENetID*/, ENetTestPlayer::State /*playerState*/> playerStates;

		//WARNING: Need more info about the map and saves etc.

		//Constructor
		State(ENetTestGame* context = NULL);

	}; State gameState, lastGameState;

	std::vector<State> gameStates;

	//WARNING: Returns the current changed states of the game
	sf::Packet getChangedStateData(const sf::Uint16 & ENetID, const unsigned int &packetType);
	void setChangedStateData(sf::Packet& packet);

	void refreshState();

	//WARNING: Returns the CURRENT frame/snapshot of the game
	sf::Packet getGameData(const sf::Uint16 & ENetID,  const unsigned int &packetType);
	void setGameData(sf::Packet& packet);
	/////////////////////////////////////////////////////////////////

	//Attributes
	const ENetwork * context;
	float dt;
	sf::RenderWindow* window;
	Camera* camera;
	std::unordered_map<sf::Uint16 /*ENetID*/, ENetTestPlayer* /*Players*/> players;
	sf::Clock clock;
	const sf::Uint16* ENetID_ptr;

	//Network and game mutex-lock
	pthread_mutex_t ENetMutex;

	//Getters
	inline const bool& getGameLoopRunning()const{return gameLoopRunning;}

	//Setters
	inline void setGameLoopRunning(const bool &state){gameLoopRunning = state;}

	//Functions
	void initTestPlayer(const sf::Uint16 *ENetID);
	void updatePlayers(const float &dt);
	void drawPlayers(sf::RenderTarget* target);
	void addPlayer(sf::Uint16 pENetID, ENetTestPlayer* player);
	void removePlayer(const sf::Uint16  &pENetID);
	void changePlayerID(const sf::Uint16 &old_p_ENetID, const sf::Uint16 &new_p_ENetID);
	void draw(sf::RenderTarget* target);
	void update(const float& dt);
	void loop();
};


#endif //ENET_TEST_GAME