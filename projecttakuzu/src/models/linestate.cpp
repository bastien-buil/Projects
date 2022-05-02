/**
* @file linestate.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "linestate.h"

LineState::LineState(int remainingWhitePawn, int remainingBlackPawn, int isLineCorrect)
{
    _remainingBlackPawn = remainingBlackPawn;
    _remainingWhitePawn = remainingWhitePawn;
    _isLineCorrect = isLineCorrect;
}

int LineState::getRemainingWhitePawn() const{

    return _remainingWhitePawn;
}

int LineState::getRemainingBlackPawn() const{

    return _remainingBlackPawn;
}

bool LineState::isCorrect() const{

    return _isLineCorrect;
}


