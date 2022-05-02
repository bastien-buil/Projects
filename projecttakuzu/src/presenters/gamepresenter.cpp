/**
* @file gamepresenter.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "gamepresenter.h"
#include "cell.h"
#include "command.h"

#include "gridreader.h"
#include "icondrawertype.h"
#include <QDialog>
#include <QMessageBox>
#include <QAbstractButton>
#include <QPushButton>
#include "gameoptionsdialog.h"
#include <QAction>
#include <QTime>


GamePresenter::GamePresenter(GameView *gameView, QObject *parent) : QObject(parent), _gameView(gameView)
{
    _gameModel = new GameModel(GridReader::getGridFromFile(6,EASY));
    _undoStack = new QUndoStack(this);
    _isGameTerminated = false;
    _timer = new Timer(this);
    restartTimer();
    connect(_timer, SIGNAL(timeChanged()), _gameView, SLOT(update()));
}

GamePresenter::~GamePresenter()
{
    delete _gameModel;
    delete _undoStack;
    delete _timer;
}


const Cell& GamePresenter::getCell(int x, int y) const{
    return _gameModel->getCell(x, y);
}

int GamePresenter::getSizeGrid() const{
    return _gameModel->getSizeGrid();
}


LineState GamePresenter::getStateRow(int rowIndex) const{
    return _gameModel->getStateRow(rowIndex);
}

LineState  GamePresenter::getStateColumn(int columnIndex) const{
    return _gameModel->getStateColumn(columnIndex);
}

void GamePresenter::redo(){
    if(_isGameTerminated) {
        return;
    }
    _undoStack->redo();
    _gameView->update();
}

void GamePresenter::undo(){
    if(_isGameTerminated) {
        return;
    }
    _undoStack->undo();
    _gameView->update();
}

void GamePresenter::nextCellValue(int x, int y){
    if(_isGameTerminated) {
        return;
    }
    if(_gameModel->isCellInGridCanbeChanged(x,y)) {
        _undoStack->push(new ChangeCellCommand(_gameModel, x, y, false));
        updateGame();
    }
}

void GamePresenter::previousCellValue(int x, int y){
    if(_isGameTerminated) {
        return;
    }
    if(_gameModel->isCellInGridCanbeChanged(x,y)) {
        _undoStack->push(new ChangeCellCommand(_gameModel, x, y, true));
       updateGame();
    }
}

void GamePresenter::showNewGameOptionsDialog() {

    GameOptionsDialog dialog(_gameModel->getSizeGrid(),_gameModel->getGridDifficulty());
    int result = dialog.exec();

    if(result == QDialog::Accepted) {
        resetGame(dialog.getSizeGridSelected(),dialog.getDifficultySelected());
    }


}

void GamePresenter::showAboutDialog() {
    QMessageBox::about(nullptr,tr("About GNU/Linux Takuzu"),tr("GNU/Linux Takuzu (c)\n Authors: Bastien Buil, J****** C**** A*****"));
}

void GamePresenter::resetGame(int newSizeGrid, Difficulty newDifficulty) {

    _gameModel = new GameModel(GridReader::getGridFromFile(newSizeGrid,newDifficulty));
    _undoStack = new QUndoStack(this);
    _isGameTerminated = false;
    restartTimer();
    _gameView->update();

}

void GamePresenter::changeIcon(QAction* action){
    IconDrawerType::IconDrawerTypeEnum iconType = (IconDrawerType::IconDrawerTypeEnum) action->data().toInt();
    IconDrawer* iconDrawer = IconDrawerType::getDrawerFromEnum(iconType);
    _gameView->setIconDrawer(iconDrawer);
    _gameView->update();
}


void GamePresenter::setTooltip(QAction* action){
    bool isTooltipDisabled = action->data().toBool();
    _gameView->setTooltipDisabled(isTooltipDisabled);
}

bool GamePresenter::isGameTerminated() {
    return _gameModel->isGameFinished();
}

void GamePresenter::showEndDialog() {

    QMessageBox winMessageBox;
    QAbstractButton *newGameButton = winMessageBox.addButton(tr("New Game?"),QMessageBox::YesRole);
    QAbstractButton *closeButton = winMessageBox.addButton("Close",QMessageBox::NoRole);
    Q_UNUSED(closeButton);
    QTime time = QTime::fromMSecsSinceStartOfDay(_timer->getCurrentTimeInMs());
    QString timeStr = time.toString("hh:mm:ss");
    QString winMessage = QString(tr("You have won in %1 !")).arg(timeStr);
    winMessageBox.setText(winMessage);
    winMessageBox.setWindowTitle("Takuzu GNU/Linux");
    winMessageBox.exec();

    if(winMessageBox.clickedButton() == newGameButton) {
        showNewGameOptionsDialog();
    }

}

void GamePresenter::updateGame() {

    _gameView->update();
    if(isGameTerminated()) {
        _timer->stopTimer();
        showEndDialog();
        _isGameTerminated = true;
    }

}

int GamePresenter::getTimeInMSecond(){
    return _timer->getCurrentTimeInMs();
}

void GamePresenter::restartTimer(){
    _timer->stopTimer();
    _timer->clearTimer();
    _timer->startTimer();
}

int GamePresenter::getNumberOfUndo(){
    return _gameModel->getUndoCount();
}

void GamePresenter::showRulesDialog() {
    QMessageBox rulesMessageBox;
    //Can't find a better way to store this text, with a define I can't provide a translation
    QString rules(tr("Takuzu, also known as Binairo, is a logic puzzle involving placement of two symbols, black and white, on a rectangular grid.\nThe objective is to fill the grid with black and white, where there is an equal number of black and white in each row and column and no more than two of either symbol adjacent to each other. Additionally, there can be no identical rows or columns.\nSource: Wikipedia"));
    rulesMessageBox.setText(rules);
    rulesMessageBox.setWindowTitle("Takuzu GNU/Linux");
    rulesMessageBox.exec();
}
