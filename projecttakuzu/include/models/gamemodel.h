/**
* @file gamemodel.h
* @author Bastien Buil
* @author J****** C**** A*****
**/
#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "grid.h"
/**
 * @brief The GameModel class organizes the Takuzu game
 */
class GameModel
{
public:
    /**
     * @brief GameModel constructor
     * @param grid The grid selected or created for the gale
     */
    GameModel(Grid grid);
    /**
     * @brief getCell returns a const ref to a cell of the grid according to its coordinates
     * Can throw an invalid_argument exception if the coordinates are not valid @see Grid::getCell
     * @param x
     * @param y
     * @return A cell of the grid
     */
    const Cell& getCell(int x, int y) const;
    /**
     * @brief getSizeGrid gives the size of the grid (it is always a square)
     * @return the size of the game grid
     */
    int getSizeGrid() const;
    /**
     * @brief getGridDifficulty gives the difficulty of the current grid
     * @return The difficulty of the game grid
     */
    Difficulty getGridDifficulty() const;
    /**
     * @brief getStateRow returns the "state" of the row given by its index @see Grid::getStateRow
     * The state checks if there is one or more errors in the given row @see LineState
     * @param rowIndex
     * @return
     */
    LineState getStateRow(int rowIndex) const;
    /**
     * @brief getStateColumn returns the "state" of the column given by its index @see Grid::getStateColumn
     * The state checks if there is one or more errors in the given column @see LineState
     * @param columnIndex
     * @return
     */
    LineState getStateColumn(int columnIndex) const;
    /**
     * @brief nextCellValue sets the cell at (x,y) coordinates to the next content value @see Grid:setCellContentToNextValue
     * @param x
     * @param y
     */
    void nextCellValue(int x, int y);
    /**
     * @brief previousCellValue sets the cell at (x,y) coordinates to the previous content value @see Grid::setCellContentToPreviousValue
     * @param x
     * @param y
     */
    void previousCellValue(int x, int y);
    /**
     * @brief isCellInGridCanbeChanged checks if a cell (given by its coordinates) can be changed (e.g its content value can be changed)
     * @param x
     * @param y
     * @return true if the content value of the given cell can be changed, false otherwise
     */
    bool isCellInGridCanbeChanged(int x, int y);
    /**
     * @brief isGameFinished tells if the game is over or not
     * The game is won if the grid is complete, with no errors.
     * @return true if the game is over, false otherwise
     */
    bool isGameFinished();
    /**
     * @brief getUndoCount gives the number of undo action the player has done so far
     * @return The number of undo in the game
     */
    int getUndoCount();
    /**
     * @brief increaseUndoCount increases the number of undo action done by the player
     */
    void increaseUndoCount();
private:
    Grid _grid;
    int _undoCount = 0;
};

#endif // GAMEMODEL_H
