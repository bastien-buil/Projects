#pragma once

#include <QString>
#include <QDateTime>


class Game
{

public:
	Game(QString gamePath, QString gameName,  QDateTime lastUpdate);
	QString getPath();
	QString getName();
	QDateTime getLastUpdate();

private:
	QString m_gamePath;
	QString m_gameName;
	QDateTime m_lastUpdate;




};

