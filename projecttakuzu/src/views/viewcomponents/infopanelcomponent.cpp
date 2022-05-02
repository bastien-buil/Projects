#include "infopanelcomponent.h"
#include "gamepresenter.h"
#include <QTime>
#include <QToolTip>

InfoPanelComponent::InfoPanelComponent()
{


}


void InfoPanelComponent::_draw(QPainter& p, GameViewState* state){
    int cellSize = state->getCellSize();
    int xInfoStart = state->getLeftPosition() + state->getNbCellPerLine() * cellSize;
    int yInfoStart = state->getTopPosition() + state->getNbCellPerLine() * cellSize;

    QRect infoPanelRect(xInfoStart, yInfoStart, cellSize * 2, cellSize * 2);
    p.setPen(Qt::black);
    p.setBrush(Qt::black);
    p.drawRect(infoPanelRect);


    QTime time = QTime::fromMSecsSinceStartOfDay(state->getPresenter()->getTimeInMSecond());
    QString timeStr = time.toString("hh:mm:ss");
    QRect timerRect(infoPanelRect.x(), infoPanelRect.y(), infoPanelRect.width(), infoPanelRect.height() / 2);
    p.setPen(Qt::red);
    p.drawText(timerRect, timeStr, Qt::AlignHCenter | Qt::AlignVCenter);


    int numberOfUndo = state->getPresenter()->getNumberOfUndo();
    QString numberOfUndoStr  = QString(tr("Number of ⎌\n%1")).arg(QString::number(numberOfUndo));
    QRect errorRect(infoPanelRect.x(), infoPanelRect.y() + infoPanelRect.height() / 2, infoPanelRect.width(), infoPanelRect.height() / 2);
    p.setPen(Qt::green);
    p.drawText(errorRect, numberOfUndoStr, Qt::AlignHCenter | Qt::AlignVCenter);
}



void InfoPanelComponent::_displayToolTip(QPoint gridPos, QPoint mouseLocalPosition, QPoint mouseGlobalPosition, QWidget* widget, GameViewState* viewState){
    GamePresenter* presenter = viewState->getPresenter();
    QRect notRedrawToolTipRect(mouseLocalPosition.x() - 3, mouseLocalPosition.y() - 3, 6, 6);
    if(gridPos.x()  == presenter->getSizeGrid() && (gridPos.y()  == presenter->getSizeGrid() || gridPos.y()  == presenter->getSizeGrid() + 1)){
        QToolTip::showText(mouseGlobalPosition, tr("Time Elapsed"), widget, notRedrawToolTipRect);
    }else if(gridPos.x()  == presenter->getSizeGrid() + 1 && (gridPos.y()  == presenter->getSizeGrid() || gridPos.y()  == presenter->getSizeGrid() + 1)){
        QToolTip::showText(mouseGlobalPosition, tr("Ctrl+Z: Undo\nCtrl+Y: Redo"), widget, notRedrawToolTipRect);
    }
}

