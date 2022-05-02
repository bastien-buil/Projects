#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include "gameviewcomponent.h"

/**
 * @brief The GridComponent class is a GameViewComponent to draw the board of the game and the pawns
 */
class GridComponent : public GameViewComponent
{
public:
    /**
     * @brief GridComponent constructor
     */
    GridComponent();
private:
    /**
     * @brief _draw Draw the board of the game
     * @param p QPainter to draw on the GameView
     * @param state State of the gameview with the information to draw on the gameview
     */
    void _draw(QPainter& p, GameViewState* state);

    /**
     * @brief drawGridLines Draw the line of the game
     * @param p QPainter to draw on the GameView
     * @param grid Rect of the cell grid
     * @param cellSize the size, in pixel, of the side of a cell
     * @param nbCellPerLine the number of cells per row and column
     */
    void drawGridLines(QPainter& p, QRect grid, int cellSize, int nbCellPerLine);

    /**
     * @brief drawGridContent Draw the pawns of the game
     * @param p QPainter to draw on the GameView
     * @param grid Rect of the cell grid
     * @param cellSize the size, in pixel, of the side of a cell
     * @param nbCellPerLine the number of cells per row and column
     * @param iconDrawer Instance of an IconDrawer to draw the pawns
     * @param selectedCell Position of the selected cell
     * @param presenter GamePresenter
     */
    void drawGridContent(QPainter& p, QRect grid, int cellSize, int nbCellPerLine, IconDrawer* iconDrawer, QPoint selectedCell, GamePresenter* presenter);
};

#endif // GRIDCOMPONENT_H
