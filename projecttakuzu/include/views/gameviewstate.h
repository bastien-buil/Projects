#ifndef GAMEVIEWSTATE_H
#define GAMEVIEWSTATE_H

#include "icondrawer.h"


class GamePresenter;

/**
 * @brief The GameViewState class is a class containing the information to render the board
 */
class GameViewState
{
public:
    /**
     * @brief GameViewState constructor
     */
    GameViewState();
    ~GameViewState();

    /**
     * @brief getGridPositionFromCoordinate returns the position in the grid from the coordinate in the widget
     * @param x coordinate on the widget
     * @param y coordinate on the widget
     * @return coordinate in the grid. Return (-1,-1) is the coordinate are before the grid.
     */
    QPoint getGridPositionFromCoordinate(int x, int y);

    /**
     * @brief getRectCoordinateFromGridPosition returns the rect with the coordinate in the widget of the cell from the position in the grid
     * @param i position in the grid
     * @param j position in the grid
     * @return rect with the coordinate of the cell
     */
    QRect getRectCoordinateFromGridPosition(int i, int j);

    /**
     * @brief getSelectedCell returns the cell hovered by the mouse
     * @return coordinate, in the grid, of the cell selected by the mouse
     */
    QPoint getSelectedCell();


    /**
     * @brief setSelectedCell Change the selected cell
     * @param newSelectedCell coordinate, in the grid, of the new cell selected by the mouse
     */
    void setSelectedCell(QPoint newSelectedCell);

    /**
     * @brief getPresenter returns the presenter of the game
     * @return
     */
    GamePresenter* getPresenter();

    /**
     * @brief setPresenter sets the presenter of the game
     * @param presenter
     */
    void setPresenter(GamePresenter *presenter);


    /**
     * @brief setIconDrawer sets the IconDrawer that will be used to draw pawns
     * @param iconDrawer
     */
    void setIconDrawer(IconDrawer *iconDrawer);

    /**
     * @brief getIconDrawer returns the IconDrawer that will be used to draw pawns
     * @return
     */
    IconDrawer* getIconDrawer();

    /**
     * @brief getMargin returns the margin between the board and the border of the widget
     * @return
     */
    int getMargin();

    /**
     * @brief getNbCellPerLine returns the number of cells per row and column
     * @return
     */
    int getNbCellPerLine();

    /**
     * @brief getCellSize returns the size, in pixel, of the side of a cell
     * @return
     */
    int getCellSize();

    /**
     * @brief getTopPosition returns the y coordinate of the top of the board
     * @return
     */
    int getTopPosition();

    /**
     * @brief getLeftPosition returns the x coordinate of the left of the board
     * @return
     */
    int getLeftPosition();

    /**
     * @brief getBoardSize returns the size total of the board
     * @return
     */
    int getBoardSize();


    /**
     * @brief getGameRect returns the rect containing all the cells
     * @return
     */
    QRect getGameRect();

    /**
     * @brief update all the values of the GameViewState from the rect of the view
     * @param viewRect Rect of the GameView Widget
     */
    void update(QRect viewRect);

private:
    GamePresenter* _presenter = nullptr;
    IconDrawer* _iconDrawer = nullptr;

    int _margin;
    int _nbCellPerLine;
    int _cellSize;

    QRect _gameRect;
    int _topPosition;
    int _leftPosition;
    int _boardSize;

    QPoint _selectedCell;

};

#endif // GAMEVIEWSTATE_H
