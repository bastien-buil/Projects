#include "Bdd.h"
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <vector>
#include <QSqlError>
#include "File.h"
#include "Game.h"
#include <QMessageBox>

Bdd::Bdd() {
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setHostName("localhost");
	db.setDatabaseName("data.db");
	if (!db.open())
	{
		throw "Error";
	}
}

Bdd::~Bdd() {
	if (db.isOpen()) {
		close();
	}
}

void Bdd::close() {
	db.close();
}

QSqlDatabase Bdd::getDb() {
	return db;
}

void Bdd::checkDb(){
	if (!db.isOpen()) {
		/*db = QSqlDatabase::addDatabase("QSQLITE");
		db.setHostName("localhost");
		db.setDatabaseName("data.db");*/
		if (!db.open())
		{
			throw "Error";
		}
	}
}

void Bdd::createTables() {
	checkDb();
	QSqlQuery query(db);
	query.exec("CREATE TABLE IF NOT EXISTS games (\
		gameName TEXT,\
		gamePath TEXT,\
		computerId TEXT,\
		lastUpdate TEXT,\
		PRIMARY KEY(gameName, computerId)\
		\
	)");
	query.exec("CREATE TABLE IF NOT EXISTS resources (\
		gameName TEXT,\
		resourcePath TEXT,\
		resourceHash TEXT,\
		resourceDate TEXT,\
		PRIMARY KEY(gameName, resourcePath)\
	)");
}

void Bdd::addGame(QString gamePath, QString gameName, QString computerId, QString lastUpdate) {
	checkDb();
	QSqlQuery query(db);
	query.prepare("INSERT INTO games (gameName, gamePath, computerId, lastUpdate) VALUES (:gameName, :gamePath, :computerId, :lastUpdate)");
	query.bindValue(":gameName", gameName);
	query.bindValue(":gamePath", gamePath);
	query.bindValue(":computerId", computerId);
	query.bindValue(":lastUpdate", lastUpdate);
	query.exec();
}


void Bdd::addGame(QString gamePath, QString gameName, QString computerId, QDateTime lastUpdate) {
	addGame(gamePath, gameName, computerId, lastUpdate.toTimeSpec(Qt::UTC).toString("yyyy - MM - dd hh:mm:ss"));
}

void Bdd::updateGameDate(QString gameName, QString computerId, QString lastUpdate) {
	checkDb();
	QSqlQuery query(db);
	query.prepare("UPDATE games SET lastUpdate = :lastUpdate WHERE gameName = :gameName AND computerId = :computerId");
	query.bindValue(":gameName", gameName);
	query.bindValue(":computerId", computerId);
	query.bindValue(":lastUpdate", lastUpdate);
	query.exec();
}


void Bdd::updateGameDate(QString gameName, QString computerId, QDateTime lastUpdate) {
	updateGameDate(gameName, computerId, lastUpdate.toTimeSpec(Qt::UTC).toString("yyyy - MM - dd hh:mm:ss"));
}

void Bdd::removeGame(QString gameName, QString computerId) {
	checkDb();
		QSqlQuery query(db);
		query.prepare("DELETE FROM games WHERE gameName = :gameName AND computerId = :computerId");
		query.bindValue(":gameName", gameName);
		query.bindValue(":computerId", computerId);
		query.exec();
}

QDateTime Bdd::getGameLastUpdate(QString gameName) {
	checkDb();
	QSqlQuery query(db);
	query.prepare("SELECT max(lastUpdate) FROM games WHERE gameName = :gameName");
	query.bindValue(":gameName", gameName);
	query.exec();
	query.next();
	QDateTime date = QDateTime::fromString(query.value(0).toString(), "yyyy - MM - dd hh:mm:ss");
	date.setTimeSpec(Qt::UTC);
	return date;
}

QList<Game> Bdd::getGames(QString computerId) {
	checkDb();
	QSqlQuery query(db);
	query.prepare("SELECT gameName, gamePath, lastUpdate FROM games WHERE computerId = :computerId");
	query.bindValue(":computerId", computerId);
	query.exec();
	QList<Game> gameList;
	while (query.next()) {
		QDateTime date = QDateTime::fromString(query.value(2).toString(), "yyyy - MM - dd hh:mm:ss");
		date.setTimeSpec(Qt::UTC);
		gameList.append(Game(query.value(1).toString(), query.value(0).toString(), date));
	}
	return gameList;
}

QList<Game> Bdd::getAllGames() {
	checkDb();
	QSqlQuery query(db);
	query.prepare("SELECT gameName, gamePath, lastUpdate FROM games GROUP BY gameName");
	query.exec();
	QList<Game> gameList;
	while (query.next()) {
		QDateTime date = QDateTime::fromString(query.value(2).toString(), "yyyy - MM - dd hh:mm:ss");
		date.setTimeSpec(Qt::UTC);
		gameList.append(Game(query.value(1).toString(), query.value(0).toString(), date));
	}
	return gameList;
}
Game Bdd::getGame(QString gameName, QString computerId) {
	checkDb();
	QSqlQuery query(db);
	query.prepare("SELECT gameName, gamePath, lastUpdate FROM games WHERE computerId = :computerId AND gameName = :gameName");
	query.bindValue(":computerId", computerId);
	query.bindValue(":gameName", gameName);
	query.exec();
	query.next();
	QDateTime date = QDateTime::fromString(query.value(2).toString(), "yyyy - MM - dd hh:mm:ss");
	date.setTimeSpec(Qt::UTC);
	return Game(query.value(1).toString(), query.value(0).toString(), date);
}


void Bdd::addPath(QString gameName, QString resourcePath, QString resourceHash, QString resourceDate) {
	checkDb();
	QSqlQuery query(db);
	query.prepare("INSERT OR REPLACE INTO resources (gameName, resourcePath, resourceHash, resourceDate) VALUES (:gameName, :resourcePath, :resourceHash, :resourceDate)");
	query.bindValue(":gameName", gameName);
	query.bindValue(":resourcePath", resourcePath);
	query.bindValue(":resourceHash", resourceHash);
	query.bindValue(":resourceDate", resourceDate);
	query.exec();
	if (query.lastError().text().length() > 0)
		QMessageBox::critical(NULL, "Error", query.lastError().text());
}

void Bdd::addPath(QString gameName, File file) {
	addPath(gameName, file.getPath(), file.getHash(), file.getDate().toTimeSpec(Qt::UTC).toString("yyyy - MM - dd hh:mm:ss"));
}


void Bdd::removePath(QString gameName, QString resourcePath) {
	checkDb();
	QSqlQuery query(db);
	query.prepare("DELETE FROM resources WHERE gameName = :gameName AND resourcePath = :resourcePath");
	query.bindValue(":gameName", gameName);
	query.bindValue(":resourcePath", resourcePath);
	query.exec();
}

void Bdd::removePath(QString gameName, File file) {
	return Bdd::removePath(gameName, file.getPath());
}

QList<File> Bdd::getPath(QString gameName) {
	checkDb();
	QSqlQuery query(db);
	query.prepare("SELECT resourcePath, resourceHash, resourceDate FROM resources WHERE gameName = :gameName");
	query.bindValue(":gameName", gameName);
	query.exec();
	if (query.lastError().text().length() > 0) 
		QMessageBox::critical(NULL, "Error", query.lastError().text());
		
	QList<File> files;
	while (query.next()) {
		QDateTime date = QDateTime::fromString(query.value(2).toString(), "yyyy - MM - dd hh:mm:ss");
		date.setTimeSpec(Qt::UTC);
		File f(query.value(0).toString(), query.value(1).toString(), date);
		files.append(f);
	}
	return files;
}