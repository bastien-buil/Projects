/**
* @file grid.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "grid.h"
#include <stdexcept>

using namespace std;

Grid::Grid(int sizeGrid, Difficulty difficulty)
{
    _sizeGrid = sizeGrid;
    _difficulty = difficulty;
    _isAtLeastOneErrorInGrid = false;
    _grid.assign(sizeGrid,vector<Cell>(sizeGrid));

}

void Grid::setCellContent(int x, int y, CELL_CONTENT content) {
    if(!checkIfInGrid(x,y)) {
        throw invalid_argument("Coords not valid for grid");
    }
    _grid.at(x).at(y).setContent(content);
}

void Grid::setCellDefinitive(int x, int y) {
    if(!checkIfInGrid(x,y)) {
        throw invalid_argument("Coords not valid for grid");
    }
    _grid.at(x).at(y).setDefinitiveCell();
}


LineState Grid::getState(int index, bool isRow) const {

    int numberOfWhitePawn = 0;
    int numberOfBlackPawn = 0;
    int maxNumberOfPawn = _sizeGrid/2;
    bool isCorrect = true;


    for(unsigned int i=0;i<_sizeGrid;i++) {

        Cell cell;
        if(isRow) {
            cell = _grid.at(index).at(i);
        } else {
            cell = _grid.at(i).at(index);
        }
        if(isCorrect && !cell.isCorrect()) {
            isCorrect = false;
        }
        switch (cell.getContent()) {
            case WHITE:
                numberOfWhitePawn++;
                break;
            case BLACK:
                numberOfBlackPawn++;
                break;
            case EMPTY_CELL:
                break;

        }
    }

    return LineState(maxNumberOfPawn-numberOfWhitePawn,maxNumberOfPawn-numberOfBlackPawn,isCorrect);



}


LineState Grid::getStateRow(int rowIndex) const{

    return getState(rowIndex,true);
}

LineState Grid::getStateColumn(int columnIndex) const{

    return getState(columnIndex,false);

}


int Grid::getSize() const{
    return _sizeGrid;
}

Difficulty Grid::getDifficulty() const{
    return _difficulty;
}

bool Grid::checkIfInGrid(unsigned int x, unsigned int y) const{
    return x < _sizeGrid && y < _sizeGrid;
}

Cell& Grid::getCell(unsigned int x, unsigned int y) {
    if(!checkIfInGrid(x,y)) {
        throw invalid_argument("Coords not valid for grid");
    }
    return _grid.at(x).at(y);
}



const Cell& Grid::getCell(unsigned int x, unsigned int y) const {

    if(!checkIfInGrid(x,y)) {
        throw invalid_argument("Coords not valid for grid");
    }
    return _grid.at(x).at(y);
}

bool Grid::isCellCanBeChanged(unsigned int x, unsigned int y) const{
    if(!checkIfInGrid(x,y)) {
        throw invalid_argument("Coords not valid for grid");
    }

    return _grid.at(x).at(y).isChangeable();
}

void Grid::resetGridCorrectness(){
    for(unsigned int x = 0; x < _sizeGrid; x++){
        for(unsigned int y = 0; y < _sizeGrid; y++){
            getCell(x, y).setCorrect(true);
        }
    }
    _isAtLeastOneErrorInGrid = false;
}

bool Grid::cellHaveSameContentHorizontalNeighboor(int x, int y){
    Cell& cell = getCell(x, y);
    return checkIfInGrid(x-1, y) && checkIfInGrid(x+1, y) &&
            getCell(x-1, y).getContent() == cell.getContent() &&
            getCell(x+1, y).getContent() == cell.getContent();
}


bool Grid::cellHaveSameContentVerticalNeighboor(int x, int y){
    Cell& cell = getCell(x, y);
    return checkIfInGrid(x, y-1) && checkIfInGrid(x, y+1) &&
            getCell(x, y-1).getContent() == cell.getContent() &&
            getCell(x, y+1).getContent() == cell.getContent();
}

void Grid::check3SameCellValue(){
    for(unsigned int x = 0; x < _sizeGrid; x++){
        for(unsigned int y = 0; y < _sizeGrid; y++){
            if(getCell(x, y).getContent() == EMPTY_CELL){
                continue;
            }
            if(cellHaveSameContentHorizontalNeighboor(x, y)){
                getCell(x-1, y).setCorrect(false);
                getCell(x, y).setCorrect(false);
                getCell(x+1, y).setCorrect(false);
                if(!_isAtLeastOneErrorInGrid) {
                    setGridInError();
                }
            }
            if(cellHaveSameContentVerticalNeighboor(x, y)){
                getCell(x, y-1).setCorrect(false);
                getCell(x, y).setCorrect(false);
                getCell(x, y+1).setCorrect(false);
                if(!_isAtLeastOneErrorInGrid) {
                    setGridInError();
                }
            }
        }
    }
}


bool Grid::isColumnFull(int x){
    for(unsigned int y = 0; y < _sizeGrid; y++){
        if(getCell(x, y).getContent() == EMPTY_CELL){
            return false;
        }
    }
    return true;
}


bool Grid::isRowFull(int y){
    for(unsigned int x = 0; x < _sizeGrid; x++){
        if(getCell(x, y).getContent() == EMPTY_CELL){
            return false;
        }
    }
    return true;
}

bool Grid::areRowsEquals(int y0, int y1){
    if(y0 == y1){
        return true;
    }
    for(unsigned int x = 0; x < _sizeGrid; x++){
        if(getCell(x, y0).getContent() != getCell(x, y1).getContent()){
            return false;
        }
    }
    return true;
}

bool Grid::areColumnsEquals(int x0, int x1){
    if(x0 == x1){
        return true;
    }
    for(unsigned int y = 0; y < _sizeGrid; y++){
        if(getCell(x0, y).getContent() != getCell(x1, y).getContent()){
            return false;
        }
    }
    return true;
}

void Grid::setColumnIncorrect(int x){
    for(unsigned int y = 0; y < _sizeGrid; y++){
        getCell(x, y).setCorrect(false);
    }

}

void Grid::setRowIncorrect(int y){
    for(unsigned int x = 0; x < _sizeGrid; x++){
        getCell(x, y).setCorrect(false);
    }
}


void Grid::checkSameRowAndColumn(){
    for(unsigned int x0 = 0; x0 < _sizeGrid; x0++){
        if(!isColumnFull(x0)){
            continue;
        }
        for(unsigned int x1 = x0 + 1; x1 < _sizeGrid; x1++){
            if(areColumnsEquals(x0, x1)){
                setColumnIncorrect(x0);
                setColumnIncorrect(x1);
                if(!_isAtLeastOneErrorInGrid) {
                    setGridInError();
                }
            }
        }
    }

    for(unsigned int y0 = 0; y0 < _sizeGrid; y0++){
        if(!isRowFull(y0)){
            continue;
        }
        for(unsigned int y1 = y0 + 1; y1 < _sizeGrid; y1++){
            if(areRowsEquals(y0, y1)){
                setRowIncorrect(y0);
                setRowIncorrect(y1);
                if(!_isAtLeastOneErrorInGrid) {
                    setGridInError();
                }
            }
        }
    }

}

void Grid::setGridInError() {
    _isAtLeastOneErrorInGrid = true;
}
void Grid::updateGrid(){
    resetGridCorrectness();
    check3SameCellValue();
    checkSameRowAndColumn();
}

bool Grid::isAtLeastOneError() {
    return _isAtLeastOneErrorInGrid;
}

bool Grid::isGridFull() {

    for(unsigned int i=0;i<_sizeGrid;i++) {
        if(!isColumnFull(i)) {
            return false;
        }
    }

    return true;

}

bool Grid::isNumberOfPawnsInLineCorrect(int index, bool isRow) {

    unsigned int numberOfWhitePawns = 0;
    unsigned int numberOfBlackPawns = 0;

    Cell cell;
    for(unsigned int i=0;i<_sizeGrid;i++) {
        if(isRow) {
            cell = _grid.at(index).at(i);
        } else {
            cell = _grid.at(i).at(index);
        }
        switch (cell.getContent()) {
            case WHITE:
                numberOfWhitePawns++;
                break;
            case BLACK:
                numberOfBlackPawns++;
                break;
            case EMPTY_CELL:
                continue;
        }

        if(numberOfBlackPawns > _sizeGrid/2 || numberOfWhitePawns > _sizeGrid/2) {
            return false;
        }
    }

    return numberOfBlackPawns == _sizeGrid/2 && numberOfWhitePawns == _sizeGrid/2;

}

bool Grid::isNumberOfPawnsInRowCorrect(int rowIndex) {
    return isNumberOfPawnsInLineCorrect(rowIndex,true);
}

bool Grid::isNumberOfPawnsInColumnCorrect(int columnIndex) {
    return isNumberOfPawnsInLineCorrect(columnIndex,false);
}

bool Grid::isNumberOfPawnsCorrectInGrid() {

    for(unsigned int i=0;i<_sizeGrid;i++) {
        if(!isNumberOfPawnsInRowCorrect(i) || !isNumberOfPawnsInColumnCorrect(i)) {
            return false;
        }
    }

    return true;
}
