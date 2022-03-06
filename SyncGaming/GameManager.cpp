#include "GameManager.h"
#include "Bdd.h"
#include "FileComparator.h"
#include "GameUpdate.h"
#include "Game.h"
#include <QString>
#include <QDateTime>
#include <QApplication>


//TO WORK https://doc.qt.io/archives/qq/qq27-responsive-guis.html
// https://doc.qt.io/qt-5/qtconcurrent.html#map

void addGame(QString dir, QString gameName) {
	Bdd db = Bdd();
	db.addGame(dir, gameName, getComputerId(), "0");
	db.close();
}

QList<GameUpdate> updateGames() {
	Bdd db = Bdd();

	QList<GameUpdate> gameUpdateList;
	QList<Game> gameList = db.getGames(getComputerId());
	while (!gameList.isEmpty()) {
		Game game = gameList.takeFirst();
		bool success = updateGame(game.getName(), db);
		gameUpdateList.append(GameUpdate(game, success));
		//QCoreApplication::processEvents();
	}
	db.close();
	return gameUpdateList;
}

QList<Game> getGames(bool getAllGame) {
	Bdd db = Bdd();
	QList<Game> gameList;
	if (getAllGame) {
		gameList = db.getAllGames();
	}else{
		gameList = db.getGames(getComputerId());
	}
	db.close();
	return gameList;
}

