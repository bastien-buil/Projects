#include "Game.h"


Game::Game(QString gamePath, QString gameName, QDateTime lastUpdate) : m_gamePath(gamePath), m_gameName(gameName), m_lastUpdate(lastUpdate)
{

}

QString Game::getPath()
{
	return m_gamePath;
}

QString Game::getName()
{
	return m_gameName;
}

QDateTime Game::getLastUpdate()
{
	return m_lastUpdate;
}
