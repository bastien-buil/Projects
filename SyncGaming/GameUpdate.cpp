#include "GameUpdate.h"
#include "Game.h"


GameUpdate::GameUpdate(Game &game, bool &updated) : m_game(game), m_updated(updated)
{
}

Game GameUpdate::getGame()
{
	return m_game;
}

bool GameUpdate::getUpdated()
{
	return m_updated;
}
