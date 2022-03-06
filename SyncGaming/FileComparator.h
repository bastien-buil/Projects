#pragma once

#include <QString>
#include <QSqlQuery>
#include "Bdd.h"


class FileComparator
{
	//public:


};

bool updateGame(QString gameName, Bdd &db);
std::tuple<QList<QString>, QList<QString>, QList<QString>, bool> checkPath(Game& game, Bdd& db);

QString getComputerId();