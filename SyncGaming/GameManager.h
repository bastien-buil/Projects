#pragma once
#include <QString>
#include <QList>
#include "GameUpdate.h"

void addGame(QString dir, QString gameName);
QList<GameUpdate> updateGames();
QList<Game> getGames(bool getAllGame);