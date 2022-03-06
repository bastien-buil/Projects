#pragma once
#include <QSqlDatabase>
#include "File.h"
#include "Game.h"

class Bdd
{


public:
	Bdd();
	~Bdd();
	void close();
	void createTables();
	void addPath(QString gameName, QString resourcePath, QString resourceHash, QString resourceDate);
	void addPath(QString gameName, File file);
	void removePath(QString gameName, QString resourcePath);
	void removePath(QString gameName, File file);
	QList<Game> getGames(QString computerId);
	QList<Game> getAllGames();
	Game getGame(QString gameName, QString computerId);
	void addGame(QString path, QString gameName, QString computerId, QString lastUpdate);
	void addGame(QString path, QString gameName, QString computerId, QDateTime lastUpdate);
	void updateGameDate(QString gameName, QString computerId, QString lastUpdate);
	void updateGameDate(QString gameName, QString computerId, QDateTime lastUpdate);
	void removeGame(QString gameName, QString computerId);
	QList<File> getPath(QString gameName);
	QDateTime getGameLastUpdate(QString gameName);

private:
	QSqlDatabase db;
	QSqlDatabase getDb();//this is now Useless
	void checkDb();


};

