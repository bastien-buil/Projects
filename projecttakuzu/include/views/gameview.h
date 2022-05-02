#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QObject>
#include <QWidget>
#include "icondrawer.h"
#include "gameviewcomponent.h"
#include "gameviewstate.h"

class GamePresenter;

/**
 * @brief The GameView class the game
 */
class GameView : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief GameView constructor
     * It also set the differents components used
     * @param parent
     */
    explicit GameView(QWidget *parent = nullptr);

    /**
     * @brief setPresenter sets the presenter of the game.
     * It should be set just after creation of the GameView.
     * @param model
     */
    void setPresenter(GamePresenter *model);

    /**
     * @brief setIconDrawer sets the icons drawer.
     * If it is not set, no pawns will be drawn on the board.
     * @param model
     */
    void setIconDrawer(IconDrawer *model);

    /**
     * @brief setTooltipDisabled Enable or disable the tooltip
     * @param isDisabled
     */
    void setTooltipDisabled(bool isDisabled);

    /**
     * @brief isTooltipDisabled returns if the tooltip is disabled.
     * @return
     */
    bool isTooltipDisabled();


protected:
    /**
     * @brief paintEvent Event executed when the widget is drawn
     * @param event
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief mouseMoveEvent Event executed when the mouse is moved.
     * Manage the tooltip and the cell selection
     * @param ev
     */
    void mouseMoveEvent(QMouseEvent* ev) override;

    /**
     * @brief mouseReleaseEvent Event executed when the mouse is released.
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    GameViewState* _viewState;

    /**
     * @brief _components Component that will be drawn of the view
     */
    QList<GameViewComponent*> _components;
    bool _tooltipDisabled = false;

signals:

};

#endif // GAMEVIEW_H
