#ifndef LINEINFOCOMPONENT_H
#define LINEINFOCOMPONENT_H


#include "gameviewcomponent.h"
#include "linestate.h"

/**
 * @brief The LineInfoComponent class is a GameViewComponent to draw the information about the lines
 */
class LineInfoComponent : public GameViewComponent
{
    Q_OBJECT
public:
    /**
     * @brief LineInfoComponent constructor
     */
    LineInfoComponent();
private:
    /**
     * @brief _draw Draw the information about the lines
     * @param p QPainter to draw on the GameView
     * @param state State of the gameview with the information to draw on the gameview
     */
    void _draw(QPainter& p, GameViewState* state);

    /**
     * @brief _displayToolTip If mouse is well placed, display a tooltip about line information.
     * @param gridPos the position of the mouse on the board
     * @param mouseLocalPosition the coordinate of the mouse in the gameview
     * @param mouseGlobalPosition the global coordinate of the mouse
     * @param widget widget on which the tooltip will be displayeds
     * @param viewState State of the gameview with the information to draw on the gameview
     */
    void _displayToolTip(QPoint gridPos, QPoint mouseLocalPosition, QPoint mouseGlobalPosition, QWidget* widget, GameViewState* viewState);

    /**
     * @brief drawCheckmark Draw a checkmark at the given position
     * @param p QPainter to draw on the GameViews
     * @param penWidth Width of the line of the checkmark
     * @param markRect Rect in which the checkmark will be drawn
     */
    void drawCheckmark(QPainter& p, int penWidth, QRect markRect);

    /**
     * @brief drawLineStateNumbers
     * @param p QPainter to draw on the GameViews
     * @param lineState the state of the line for which the information should be written
     * @param whiteRect the rect in which should be written the count of white pawns
     * @param blackRect the rect in which should be written the count of black pawns
     */
    void drawLineStateNumbers(QPainter& p, const LineState& lineState, QRect whiteRect, QRect blackRect);

    /**
     * @brief drawLineInfoOfLine Draw line information of a row
     * @param p QPainter to draw on the GameViews
     * @param state LineState of the row
     * @param i The index of the row
     * @param cellSize the size, in pixel, of the side of a cell
     * @param xInfoStart the x coordinate of the left of the line info
     * @param topPosition the y coordinate of the top of the board
     * @param marginSize Margin for the line that separate black and white cell information
     */
    void drawLineInfoOfLine(QPainter& p, GameViewState* state, int i, int cellSize, int xInfoStart, int topPosition, int marginSize);

    /**
     * @brief drawLineInfoOfColumn Draw line information of a column
     * @param p QPainter to draw on the GameViews
     * @param state LineState of the column
     * @param i The index of the column
     * @param cellSize the size, in pixel, of the side of a cell
     * @param yInfoStart the y coordinate of the top of the line info
     * @param leftPosition the x coordinate of the left of the board
     * @param marginSize Margin for the line that separate black and white cell information
     */
    void drawLineInfoOfColumn(QPainter& p, GameViewState* state, int i, int cellSize, int yInfoStart, int leftPosition, int marginSize);
};

#endif // LINEINFOCOMPONENT_H
