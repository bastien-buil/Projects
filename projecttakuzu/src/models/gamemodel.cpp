/**
* @file gamemodel.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "gamemodel.h"

GameModel::GameModel(Grid grid) : _grid(grid)
{

}


const Cell& GameModel::getCell(int x, int y) const{
    return _grid.getCell(x, y);
}


int GameModel::getSizeGrid() const{
    return _grid.getSize();
}

Difficulty GameModel::getGridDifficulty() const {

    return _grid.getDifficulty();

}

LineState GameModel::getStateRow(int rowIndex) const {
    return _grid.getStateRow(rowIndex);
}

LineState GameModel::getStateColumn(int columnIndex) const{
    return _grid.getStateColumn(columnIndex);
}


void GameModel::nextCellValue(int x, int y){
    _grid.getCell(x, y).setCellContentToNextValue();
    _grid.updateGrid();
}

void GameModel::previousCellValue(int x, int y){
    _grid.getCell(x, y).setCellContentToPreviousValue();
    _grid.updateGrid();
}

bool GameModel::isCellInGridCanbeChanged(int x, int y) {

    return _grid.isCellCanBeChanged(x,y);
}

bool GameModel::isGameFinished() {

        return !_grid.isAtLeastOneError() && _grid.isGridFull() && _grid.isNumberOfPawnsCorrectInGrid();

}

int GameModel::getUndoCount(){
    return _undoCount;
}

void GameModel::increaseUndoCount(){
    _undoCount++;
}

