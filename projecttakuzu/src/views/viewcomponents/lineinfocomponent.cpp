#include "lineinfocomponent.h"
#include "linestate.h"
#include "gamepresenter.h"
#include <QPen>
#include <QToolTip>
#include <QPainterPath>

LineInfoComponent::LineInfoComponent()
{

}


void LineInfoComponent::drawCheckmark(QPainter& p, int penWidth, QRect markRect){
    QPen pen(Qt::green);
    pen.setWidth(penWidth);
    p.setPen(pen);
    QPainterPath path;
    path.moveTo(markRect.x(), markRect.y() + markRect.height()/2);
    path.lineTo(markRect.x() + markRect.width() / 2, markRect.y() + markRect.height());
    path.lineTo(markRect.x() + markRect.width(), markRect.y());
    p.drawPath(path);
}


void LineInfoComponent::drawLineStateNumbers(QPainter& p, const LineState& lineState, QRect whiteRect, QRect blackRect){
    p.setPen(lineState.getRemainingWhitePawn() < 0 ? Qt::red : Qt::black);
    p.drawText(whiteRect, QString::number(lineState.getRemainingWhitePawn()), Qt::AlignHCenter | Qt::AlignVCenter);

    p.setPen(lineState.getRemainingBlackPawn() < 0 ? Qt::red : Qt::black);
    p.drawText(blackRect, QString::number(lineState.getRemainingBlackPawn()), Qt::AlignHCenter | Qt::AlignVCenter);
}


void LineInfoComponent::drawLineInfoOfLine(QPainter& p, GameViewState* state, int i, int cellSize, int xInfoStart, int topPosition, int marginSize){
    QRect leftInfoRect(xInfoStart, topPosition + cellSize * i, cellSize, cellSize);
    QRect rightInfoRect(xInfoStart + cellSize, topPosition + cellSize * i, cellSize, cellSize);
    p.setBrush(Qt::transparent);
    p.setPen(Qt::black);
    const LineState& rowState = state->getPresenter()->getStateRow(i);
    if(rowState.isCorrect() && rowState.getRemainingBlackPawn() == 0 && rowState.getRemainingWhitePawn() == 0){
        QRect markRect(leftInfoRect.x() + 3*leftInfoRect.width()/4, leftInfoRect.y() + leftInfoRect.height()/4, leftInfoRect.width() / 2, leftInfoRect.height()/2);
        drawCheckmark(p, leftInfoRect.width()/20, markRect);
    }else{
        if(!rowState.isCorrect()){
            p.setPen(Qt::red);
        }
        p.drawLine(xInfoStart + cellSize, topPosition + cellSize * i + marginSize,  xInfoStart + cellSize, topPosition + cellSize * (i+1) - marginSize);
        drawLineStateNumbers(p, rowState,  leftInfoRect, rightInfoRect);
    }
}


void LineInfoComponent::drawLineInfoOfColumn(QPainter& p, GameViewState* state, int i, int cellSize, int yInfoStart, int leftPosition, int marginSize){
    QRect topInfoRect(leftPosition + cellSize * i, yInfoStart, cellSize, cellSize);
    QRect bottomInfoRect(leftPosition + cellSize * i, yInfoStart + cellSize, cellSize, cellSize);
    p.setBrush(Qt::transparent);
    p.setPen(Qt::black);
    const LineState& columnState = state->getPresenter()->getStateColumn(i);
    if(columnState.isCorrect() && columnState.getRemainingBlackPawn() == 0 && columnState.getRemainingWhitePawn() == 0){
        QRect markRect(topInfoRect.x() + topInfoRect.width()/4, topInfoRect.y() + 3 * topInfoRect.height() / 4, topInfoRect.width() / 2, topInfoRect.height()/2);
        drawCheckmark(p, topInfoRect.width()/20, markRect);
    }else{
        if(!columnState.isCorrect()){
            p.setPen(Qt::red);
        }
        p.drawLine(topInfoRect.x() + marginSize, yInfoStart + cellSize, leftPosition + cellSize * (i+1) - marginSize, yInfoStart + cellSize);
        drawLineStateNumbers(p, columnState,  topInfoRect, bottomInfoRect);
    }
}


void LineInfoComponent::_draw(QPainter& p, GameViewState* state){

    int leftPosition = state->getLeftPosition();
    int topPosition = state->getTopPosition();
    int nbCellPerLine = state->getNbCellPerLine();
    int cellSize = state->getCellSize();

    int xInfoStart = leftPosition + nbCellPerLine * cellSize;
    int yInfoStart = topPosition + nbCellPerLine * cellSize;
    int marginSize = cellSize / 10;

    for(int i = 0; i < nbCellPerLine; i++){
        drawLineInfoOfLine(p, state, i, cellSize, xInfoStart, topPosition, marginSize);
        drawLineInfoOfColumn(p, state, i, cellSize, yInfoStart, leftPosition, marginSize);
    }
}


void LineInfoComponent::_displayToolTip(QPoint gridPos, QPoint mouseLocalPosition, QPoint mouseGlobalPosition, QWidget* widget, GameViewState* viewState){
    GamePresenter* presenter = viewState->getPresenter();
    QRect notRedrawToolTipRect(mouseLocalPosition.x() - 3, mouseLocalPosition.y() - 3, 6, 6);
    if((gridPos.y() < presenter->getSizeGrid() && presenter->getSizeGrid() == gridPos.x()) ||
           (gridPos.x() < presenter->getSizeGrid() && presenter->getSizeGrid() == gridPos.y())){
        QToolTip::showText(mouseGlobalPosition, tr("Number of remaining of white pawns"), widget, notRedrawToolTipRect);
    }else if((gridPos.y() < presenter->getSizeGrid() && presenter->getSizeGrid() + 1 == gridPos.x()) ||
             (gridPos.x() < presenter->getSizeGrid() && presenter->getSizeGrid() + 1 == gridPos.y())){
        QToolTip::showText(mouseGlobalPosition, tr("Number of remaining of black pawns"), widget, notRedrawToolTipRect);
    }
}
