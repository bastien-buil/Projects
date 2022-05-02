/**
* @file command.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "command.h"


ChangeCellCommand::ChangeCellCommand(GameModel* model,int x, int y) : _model(model), _x(x), _y(y), _isReversed(false) {

}

ChangeCellCommand::ChangeCellCommand(GameModel* model,int x, int y, bool isReversed) : _model(model), _x(x), _y(y), _isReversed(isReversed) {

}


void ChangeCellCommand::redo(){
    if(_isReversed){
        _model->previousCellValue(_x, _y);
    }else{
        _model->nextCellValue(_x, _y);
    }
}

void ChangeCellCommand::undo(){
    if(_isReversed){
        _model->nextCellValue(_x, _y);
    }else{
        _model->previousCellValue(_x, _y);
    }
    _model->increaseUndoCount();
}

