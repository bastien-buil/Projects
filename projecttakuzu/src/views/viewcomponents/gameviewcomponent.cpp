#include "gameviewcomponent.h"

void GameViewComponent::draw(QPainter& p, GameViewState* state){
    if(_isDisabled){
        return;
    }

    _draw(p, state);
}


void GameViewComponent::displayToolTip(QPoint gridPos, QPoint mouseLocalPosition, QPoint mouseGlobalPosition, QWidget* widget, GameViewState* viewState){
    if(_isDisabled){
        return;
    }

    _displayToolTip(gridPos, mouseLocalPosition, mouseGlobalPosition, widget, viewState);
}

void GameViewComponent::setDisabled(bool isDisabled){
    _isDisabled = isDisabled;
}


bool GameViewComponent::getDisabled(){
    return _isDisabled;
}
