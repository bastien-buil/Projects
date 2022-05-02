#include "gameviewstate.h"
#include "gamepresenter.h"

GameViewState::GameViewState()
{
    _selectedCell = QPoint(-1, -1);
    _margin = 10;
}

void GameViewState::update(QRect viewRect){
    _boardSize = (viewRect.width() < viewRect.height() ? viewRect.width() : viewRect.height()) - 2 * _margin;
    _topPosition = viewRect.y() + (viewRect.height() - _boardSize) / 2;
    _leftPosition = viewRect.x() + (viewRect.width() - _boardSize) / 2;
    _gameRect = QRect(_leftPosition, _topPosition, _boardSize, _boardSize);
    _cellSize = _boardSize / (_presenter->getSizeGrid() + 2);
    _nbCellPerLine = _presenter->getSizeGrid();
}

void GameViewState::setIconDrawer(IconDrawer *iconDrawer){
    if(_iconDrawer != nullptr){
        delete _iconDrawer;
    }
    _iconDrawer = iconDrawer;
}

IconDrawer* GameViewState::getIconDrawer(){
    return _iconDrawer;
}

GamePresenter* GameViewState::getPresenter(){
    return _presenter;
}


void GameViewState::setPresenter(GamePresenter *presenter){
    _presenter = presenter;
}

QPoint GameViewState::getGridPositionFromCoordinate(int x, int y){
    int xRelative = (x - _leftPosition);
    int yRelative = (y - _topPosition);
    int i = yRelative / _cellSize;
    int j = xRelative / _cellSize;
    if(xRelative < 0  || yRelative < 0 ){
        i = -1;
        j = -1;
    }
    return QPoint(i, j);
}


QRect GameViewState::getRectCoordinateFromGridPosition(int i, int j){
    if(i == -1 || j == -1){
        throw "Invalid Grid Position";
    }

    QRect newGame(i * _cellSize + _leftPosition, j * _cellSize + _topPosition, _cellSize, _cellSize);
    return newGame;
}

QPoint GameViewState::getSelectedCell(){
    return _selectedCell;
}

void GameViewState::setSelectedCell(QPoint newSelectedCell){
    _selectedCell = newSelectedCell;
}


int GameViewState::getMargin(){
    return _margin;
}

int GameViewState::getNbCellPerLine(){
    return _nbCellPerLine;
}

int GameViewState::getCellSize(){
    return _cellSize;
}

int GameViewState::getTopPosition(){
    return _topPosition;
}

int GameViewState::getLeftPosition(){
    return _leftPosition;
}

int GameViewState::getBoardSize(){
    return _boardSize;
}

QRect GameViewState::getGameRect(){
    return _gameRect;
}
