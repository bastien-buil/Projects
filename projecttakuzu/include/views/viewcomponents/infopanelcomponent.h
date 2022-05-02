#ifndef INFOPANELCOMPONENT_H
#define INFOPANELCOMPONENT_H


#include "gameviewcomponent.h"

/**
 * @brief The InfoPanelComponent class is a GameViewComponent to draw the bottom-right information panel
 */
class InfoPanelComponent: public GameViewComponent
{
    Q_OBJECT
public:
    /**
     * @brief InfoPanelComponent constructor
     */
    InfoPanelComponent();
private:
    /**
     * @brief _draw Draw the bottom-right information panel
     * @param p QPainter to draw on the GameView
     * @param state State of the gameview with the information to draw on the gameview
     */
    void _draw(QPainter& p, GameViewState* state);

    /**
     * @brief _displayToolTip If mouse is well placed, display a tooltip about information panel.
     * @param gridPos the position of the mouse on the board
     * @param mouseLocalPosition the coordinate of the mouse in the gameview
     * @param mouseGlobalPosition the global coordinate of the mouse
     * @param widget widget on which the tooltip will be displayeds
     * @param viewState State of the gameview with the information to draw on the gameview
     */
    void _displayToolTip(QPoint gridPos, QPoint mouseLocalPosition, QPoint mouseGlobalPosition, QWidget* widget, GameViewState* viewState);
};

#endif // INFOPANELCOMPONENT_H
