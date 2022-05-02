#include "gameview.h"
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include <QPainterPath>
#include <QTime>
#include <QToolTip>
#include <QList>
#include "gamepresenter.h"
#include "gridcomponent.h"
#include "lineinfocomponent.h"
#include "infopanelcomponent.h"

GameView::GameView(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    _components = QList<GameViewComponent*>();
    _viewState = new GameViewState();

    _components.append(new GridComponent);
    _components.append(new LineInfoComponent);
    _components.append(new InfoPanelComponent);
}


void GameView::setPresenter(GamePresenter *presenter){
    _viewState->setPresenter(presenter);
}

void GameView::setIconDrawer(IconDrawer *iconDrawer){
    _viewState->setIconDrawer(iconDrawer);
}


void GameView::mouseMoveEvent(QMouseEvent* ev){
    GamePresenter* presenter = _viewState->getPresenter();
    QPoint newCoord = ev->localPos().toPoint();
    QPoint newPos = _viewState->getGridPositionFromCoordinate(newCoord.x(), newCoord.y());
    if(newPos != _viewState->getSelectedCell()){
        _viewState->setSelectedCell(newPos);
        update();
    }

    if(!_tooltipDisabled && newPos.x() != -1 && newPos.y() != -1 && newPos.x() < presenter->getSizeGrid() + 2 && newPos.y() < presenter->getSizeGrid() + 2){
        QPoint mousePosition = ev->globalPos();
        QPoint localPosition = this->mapFromGlobal(mousePosition);
        for (QList<GameViewComponent*>::iterator it = _components.begin(); it != _components.end(); it++){
            (*it)->displayToolTip(newPos, localPosition, mousePosition, this, _viewState);
        }
    }
}



void GameView::mouseReleaseEvent(QMouseEvent *ev){
    GamePresenter* presenter = _viewState->getPresenter();
    QPoint newPos = _viewState->getGridPositionFromCoordinate(ev->localPos().toPoint().x(), ev->localPos().toPoint().y());
    if(newPos.x() != -1 && newPos.y() != -1 && newPos.x() < presenter->getSizeGrid() && newPos.y() < presenter->getSizeGrid()){
        if(ev->button() == Qt::LeftButton){
            presenter->nextCellValue(newPos.x(), newPos.y());
        }else if(ev->button() == Qt::RightButton){
            presenter->previousCellValue(newPos.x(), newPos.y());
        }
    }
}




void GameView::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect = event->rect();
    painter.setRenderHint(QPainter::Antialiasing, true);

   _viewState->update(rect);


    QRect gameRect = _viewState->getGameRect();
    painter.fillRect(gameRect.x(), gameRect.y(), gameRect.width(), gameRect.height(), Qt::white);

    for (QList<GameViewComponent*>::iterator it = _components.begin(); it != _components.end(); it++){
        (*it)->draw(painter, _viewState);
    }
}




void GameView::setTooltipDisabled(bool isDisabled){
    _tooltipDisabled = isDisabled;
}

bool GameView::isTooltipDisabled(){
    return _tooltipDisabled;
}
