#include "GameInfoWindow.h"
#include "Bdd.h"
#include "FileComparator.h"
#include <QString>

GameInfoWindow::GameInfoWindow(QString gameName)
	: QWidget()
{
	Bdd db = Bdd();
	Game game = db.getGame(gameName, getComputerId());
	std::tuple<QList<QString>, QList<QString>, QList<QString>, bool> tp(checkPath(game, db));
	db.close();
	m_label = new QLabel("\n" + gameName +
		"\n Last Local Game Update: " + game.getLastUpdate().toString() +
		"\n Last Global Game Update: " + db.getGameLastUpdate(game.getName()).toString() +
		"\n UpdatedList: " + QString::number(std::get<0>(tp).size()) +
		"\n olderList: " + QString::number(std::get<1>(tp).size()) +
		"\n unknownList: " + QString::number(std::get<2>(tp).size()), this);
}

GameInfoWindow::~GameInfoWindow()
{
}
