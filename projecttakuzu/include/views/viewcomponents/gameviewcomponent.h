#ifndef GAMEVIEWCOMPONENT_H
#define GAMEVIEWCOMPONENT_H

#include "gameviewstate.h"
#include <QPainter>

class GameViewComponent: public QObject
{
    Q_OBJECT
public:

    /**
     * @brief _draw If the component is not disabled, call _draw to draw the component on the gameview
     * @param p QPainter to draw on the GameView
     * @param state State of the gameview with the information to draw on the gameview.
     */
    void draw(QPainter& p, GameViewState* state);

    /**
     * @brief displayToolTip If the component is not disabled, call _displayToolTip to create tooltip for the components
     * @param gridPos the position of the mouse on the board
     * @param mouseLocalPosition the coordinate of the mouse in the gameview
     * @param mouseGlobalPosition the global coordinate of the mouse
     * @param widget widget on which the tooltip will be displayeds
     * @param viewState State of the gameview with the information to draw on the gameview
     */
    void displayToolTip(QPoint gridPos, QPoint mouseLocalPosition, QPoint mouseGlobalPosition, QWidget* widget, GameViewState* viewState);

    /**
     * @brief setDisabled set if the component is disabled.
     * @param isDisabled
     */
    void setDisabled(bool isDisabled);

    /**
     * @brief getDisabled returns if the component is disabled.
     * @return
     */
    bool getDisabled();

private:
    bool _isDisabled = false;

    /**
     * @brief _draw It should be overrided to draw the component on the GameView
     * @param p QPainter to draw on the GameView
     * @param state State of the gameview with the information to draw on the gameview
     */
    virtual void _draw(QPainter& p, GameViewState* state) = 0;

    /**
     * @brief _displayToolTip It can be overrided to display tooltip for the component
     * @param gridPos the position of the mouse on the board
     * @param mouseLocalPosition the coordinate of the mouse in the gameview
     * @param mouseGlobalPosition the global coordinate of the mouse
     * @param widget widget on which the tooltip will be displayeds
     * @param viewState State of the gameview with the information to draw on the gameview
     */
    virtual void _displayToolTip(QPoint gridPos, QPoint mouseLocalPosition, QPoint mouseGlobalPosition, QWidget* widget, GameViewState* viewState) {
        Q_UNUSED(gridPos);
        Q_UNUSED(mouseLocalPosition);
        Q_UNUSED(mouseGlobalPosition);
        Q_UNUSED(widget);
        Q_UNUSED(viewState);
    };
};

#endif // GAMEVIEWCOMPONENT_H
