/**
* @file cell.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "cell.h"

Cell::Cell()
{
    _content = EMPTY_CELL;
    _isChangeable = true;
    _isCorrectInGrid = true;

}

bool Cell::isCorrect() const {
    return _isCorrectInGrid;
}

void Cell::setContent(CELL_CONTENT content) {

    _content = content;

}

void Cell::setCorrect(bool isCorrect){
    _isCorrectInGrid = isCorrect;
}

void Cell::setDefinitiveCell() {
    _isChangeable = false;
}


CELL_CONTENT Cell::getContent() const {

    return _content;
}

bool Cell::isChangeable() const{
    return _isChangeable;
}

CELL_CONTENT Cell::getNextCellContent(){
    return (CELL_CONTENT) ((_content + 1)  %  3);
}

void Cell::setCellContentToNextValue(){
    setContent(getNextCellContent());
}

CELL_CONTENT Cell::getPreviousCellContent(){
    if(_content == 0){
        return (CELL_CONTENT) 2;
    }else{
        return (CELL_CONTENT) ((_content - 1)  %  3);
    }
}

void Cell::setCellContentToPreviousValue(){
    setContent(getPreviousCellContent());
}

