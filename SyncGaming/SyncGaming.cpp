#include "SyncGaming.h"
#include "FileComparator.h"
#include "GameManager.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel> 
#include <QTableWidget> 
#include <QTableWidgetItem> 
#include <QtConcurrent> 
#include "GameInfoWindow.h"
#include "GameUpdate.h"
#include <QCheckBox>

SyncGaming::SyncGaming(QWidget *parent)
    : QMainWindow(parent)
{
    m_mainWidget = new QWidget;

    m_computerIdLabel = new QLabel("Computer Id: " + getComputerId());
    m_lineGame = new QLineEdit(this);
    m_addGameButton = new QPushButton("AddGame");
    connect(m_addGameButton, SIGNAL(pressed()), this, SLOT(onAddGame()));


    m_updateGamesButton = new QPushButton("Update Games");
    connect(m_updateGamesButton, SIGNAL(pressed()), this, SLOT(onUpdateGames()));
    m_displayGamesButton = new QPushButton("Display Games");
    connect(m_displayGamesButton, SIGNAL(pressed()), this, SLOT(onDisplayGames()));
    m_displayAllGamesCheck = new QCheckBox("Display All Games");

    m_tableWidget = new QTableWidget();
    m_tableWidget->setColumnCount(3);
    m_tableWidget->setHorizontalHeaderLabels(QStringList() << "Name" << "Path" << "Updated");
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(m_tableWidget, SIGNAL(cellPressed(int, int)), this, SLOT(onGameClick(int, int)));

    m_layout = new QHBoxLayout;

    m_layoutRight = new QVBoxLayout;
    m_layoutLeft = new QVBoxLayout;

    m_layoutLeft->addWidget(m_computerIdLabel);
    m_layoutLeft->addWidget(m_lineGame);
    m_layoutLeft->addWidget(m_addGameButton);


    m_layoutRight->addWidget(m_updateGamesButton);
    m_layoutRight->addWidget(m_displayGamesButton);
    m_layoutRight->addWidget(m_displayAllGamesCheck);
    m_layoutRight->addWidget(m_tableWidget);


    m_layout->addLayout(m_layoutLeft);
    m_layout->addLayout(m_layoutRight);
    m_mainWidget->setLayout(m_layout);


    this->setCentralWidget(m_mainWidget);

    m_watcherFutureGame = new QFutureWatcher<QList<Game>>(this);
    connect(m_watcherFutureGame, &QFutureWatcher<QList<Game>>::finished, this, &SyncGaming::onDisplayGamesFinished);

    m_watcherFutureGameUpdate = new QFutureWatcher<QList<GameUpdate>>(this);
    connect(m_watcherFutureGameUpdate, &QFutureWatcher<QList<GameUpdate>>::finished, this, &SyncGaming::onUpdateGamesFinished);

    onDisplayGames();
}


void SyncGaming::onAddGame() {

    QString dossier = QFileDialog::getExistingDirectory(this);
    if(dossier != ""){
        addGame(dossier, m_lineGame->text().toLower());
        QMessageBox::information(NULL, "Game added", "Game added: " + m_lineGame->text().toLower());
    }
}



void SyncGaming::onGameClick(int row, int column) {

    QString gameName = m_tableWidget->item(row, 0)->text();
    if (!gameName.isEmpty()) {
        delete m_gameWindow;
        m_gameWindow = new GameInfoWindow(gameName);
        m_gameWindow->show();
    }

}

void SyncGaming::onUpdateGamesFinished() {
    updateTableGU(m_watcherFutureGameUpdate->result());
}

void SyncGaming::onUpdateGames() {
    if (!m_watcherFutureGameUpdate->isFinished()) {
        return;
    }
    QFuture<QList<GameUpdate>> futureGameUpdate = QtConcurrent::run(updateGames);
    m_watcherFutureGameUpdate->setFuture(futureGameUpdate);
}



void SyncGaming::updateTableGU(QList<GameUpdate> list) {
    m_tableWidget->clearContents();
    m_tableWidget->setRowCount(0);
    for (int i = 0; i < list.count(); ++i)
    {

        GameUpdate gameUpdate = list.at(i);
        int row = m_tableWidget->rowCount();
        m_tableWidget->insertRow(row);
        QTableWidgetItem* nameItem = new QTableWidgetItem(gameUpdate.getGame().getName());
        m_tableWidget->setItem(row, 0, nameItem);
        QTableWidgetItem* pathItem = new QTableWidgetItem(gameUpdate.getGame().getPath());
        m_tableWidget->setItem(row, 1, pathItem);
        QTableWidgetItem* updatedItem = new QTableWidgetItem(gameUpdate.getUpdated() ? "Updated" : "Can't Update");
        m_tableWidget->setItem(row, 2, updatedItem);

    }
}

void SyncGaming::updateTableG(QList<Game> list) {
    m_tableWidget->clearContents();
    m_tableWidget->setRowCount(0);
    for (int i = 0; i < list.count(); ++i)
    {

        Game game = list.at(i);
        int row = m_tableWidget->rowCount();
        m_tableWidget->insertRow(row);
        QTableWidgetItem* nameItem = new QTableWidgetItem(game.getName());
        m_tableWidget->setItem(row, 0, nameItem);
        QTableWidgetItem* pathItem = new QTableWidgetItem(game.getPath());
        m_tableWidget->setItem(row, 1, pathItem);
        QTableWidgetItem* updatedItem = new QTableWidgetItem("");
        m_tableWidget->setItem(row, 2, updatedItem);
    }
}

void SyncGaming::onDisplayGamesFinished() {
    updateTableG(m_watcherFutureGame->result());
}

void SyncGaming::onDisplayGames() {
    if (!m_watcherFutureGame->isFinished()) {
        return;
    }
    bool displayAllGame = m_displayAllGamesCheck->checkState() == Qt::Checked ? true : false;

    QFuture<QList<Game>> futureGame = QtConcurrent::run(getGames, displayAllGame);
    m_watcherFutureGame->setFuture(futureGame);
}



