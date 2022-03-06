#pragma once

#include <QWidget>
#include <QLabel>

class GameInfoWindow : public QWidget
{
	Q_OBJECT

public:
	GameInfoWindow(QString gameName);
	~GameInfoWindow();

private:
	QLabel *m_label;
};
