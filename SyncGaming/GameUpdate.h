#pragma once

#pragma once

#include <QString>
#include <QDateTime>

#include "Game.h"

class GameUpdate
{

public:
	GameUpdate(Game &game, bool &updated);
	Game getGame();
	bool getUpdated();
private:
	Game m_game;
	bool m_updated;




};

