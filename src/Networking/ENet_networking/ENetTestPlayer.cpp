#include "ENetTestPlayer.h"

sf::Packet& operator <<(sf::Packet& packet, const ENetTestPlayer::State& state)
{
	packet <<
	(sf::Uint8)state.playerSnapshot.type <<
	//StartPosition
	//(float)state.playerSnapshot.startPos.x <<
	//(float)state.playerSnapshot.startPos.y <<
	//Velocity
	//(float)state.playerSnapshot.velocity.x <<
	//(float)state.playerSnapshot.velocity.y <<
	//Endpos
	(float)state.playerSnapshot.endPos.x <<
	(float)state.playerSnapshot.endPos.y;

	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, ENetTestPlayer::State &state)
{
	packet >>
	state.playerSnapshot.type >>
	//StartPosition
	//state.playerSnapshot.startPos.x >>
	//state.playerSnapshot.startPos.y >>
	//Velocity
	//state.playerSnapshot.velocity.x >>
	//state.playerSnapshot.velocity.y >>
	//Endpos
	state.playerSnapshot.endPos.x >>
	state.playerSnapshot.endPos.y;

	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const ENetTestPlayer* player)
{
	packet <<
	(std::string)player->playerID <<
	(sf::Uint8)player->currentStateType <<
	//Color
	(sf::Uint8)player->rect.getFillColor().r <<
	(sf::Uint8)player->rect.getFillColor().g <<
	(sf::Uint8)player->rect.getFillColor().b <<
	//StartPosition
	(float)player->rect.getPosition().x <<
	(float)player->rect.getPosition().y <<
	//Velocity
	(float)player->velocity.x <<
	(float)player->velocity.y <<
	//Endpos
	(float)player->endPos.x <<
	(float)player->endPos.y;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, ENetTestPlayer* player)
{
	std::string pPlayerID;
	sf::Uint8 pCurrentStateType;
	sf::Uint8 r, g, b;
	sf::Vector2f pPos;
	sf::Vector2f pVel;
	sf::Vector2f pEndPos;

	packet >>
	pPlayerID >>
	pCurrentStateType >>
	//Color
	r >> g >> b >>
	//StartPosition
	pPos.x >> pPos.y >>
	//Velocity
	pVel.x >> pVel.y >>
	//Endpos
	pEndPos.x >> pEndPos.y;

	delete player;
	player = new ENetTestPlayer(pPlayerID, pPos, sf::Color(r, g, b),
		 (ENetTestPlayer::StateType)pCurrentStateType, pVel, pEndPos);

	return packet;
}