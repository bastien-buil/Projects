#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QTableWidget> 
#include <QCheckBox>
#include <QtConcurrent>
#include "GameInfoWindow.h"
#include "GameUpdate.h"
#include "Game.h"


class SyncGaming : public QMainWindow
{
    Q_OBJECT

public:
    SyncGaming(QWidget *parent = Q_NULLPTR);
    void updateTableGU(QList<GameUpdate> list);
    void updateTableG(QList<Game> list);

public slots:
    void onAddGame();
    void onUpdateGames();
    void onUpdateGamesFinished();
    void onDisplayGames();
    void onDisplayGamesFinished();
    void onGameClick(int row, int column);

private:
    QWidget* m_mainWidget;
    QHBoxLayout* m_layout;


    QVBoxLayout* m_layoutLeft;
    QLineEdit *m_lineGame;
    QPushButton *m_addGameButton;
    QLabel *m_computerIdLabel;

    QVBoxLayout* m_layoutRight;
    QPushButton *m_updateGamesButton;
    QPushButton* m_displayGamesButton;
    QCheckBox* m_displayAllGamesCheck;
    QTableWidget *m_tableWidget;
    GameInfoWindow* m_gameWindow = NULL;


    QFutureWatcher<QList<GameUpdate>>* m_watcherFutureGameUpdate;
    QFutureWatcher<QList<Game>>* m_watcherFutureGame;


};
