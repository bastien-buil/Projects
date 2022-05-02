/**
* @file grid.h
* @author Bastien Buil
* @author J****** C**** A*****
**/
#ifndef GRID_H
#define GRID_H

#include <vector>
#include "cell.h"
#include "linestate.h"

/**
 * @brief The Difficulty enum represents the two level of difficulty available for the Takuzu
 */
enum Difficulty {
    EASY,
    HARD,
};
/**
 * @brief The Grid class represents a grid of Takuzu
 */
class Grid
{
public:
    /**
     * @brief Grid constructor
     * By default all the cells are empty cells and all cell can be modified.
     * To change a cell @see setCellContent or @see setCellDefinitive
     * @param sizeGrid The size of the grid
     * @param difficulty The difficulty
     */
    Grid(int sizeGrid, Difficulty difficulty);
    /**
     * @brief getStateRow returns the "state" of the row given by its index
     * The state checks if there is one or more errors in the given row @see LineState
     * @param rowIndex
     * @return
     */
    LineState getStateRow(int rowIndex) const;
    /**
     * @brief getStateColumn returns the "state" of the column given by its index
     * The state checks if there is one or more errors in the given column @see LineState
     * @param columnIndex
     * @return
     */
    LineState getStateColumn(int columnIndex) const;
    /**
     * @brief setCellContent sets the cell content of the (x,y) cell with the content provided @see Cell::setContent
     * If the coordinates are not correct, an exception will be thrown
     * @param x
     * @param y
     * @param content
     */
    void setCellContent(int x, int y, CELL_CONTENT content);
    /**
     * @brief setCellDefinitive sets the cell with (x,y) coordinates to be not mutable @see Cell:setDefinitiveCell
     * If the coordinates are not correct, an exception will be thrown
     * @param x
     * @param y
     */
    void setCellDefinitive(int x, int y);
    /**
     * @brief getCell returns a reference to a cell (given by its coordinates) in the grid
     * If the coordinates are not correct, an exception will be thrown
     * @param x
     * @param y
     * @return
     */
    Cell& getCell(unsigned int x, unsigned int y);
    /**
     * @brief getCell returns a const reference to a cell (given by its coordinates) in the grid
     * If the coordinates are not correct, an exception will be thrown
     * @param x
     * @param y
     * @return
     */
    const Cell& getCell(unsigned int x, unsigned int y) const;
    /**
     * @brief isCellCanBeChanged checks if a cell (at (x,y) coordinates) can be changed (eg if its content can be changed) @see Cell::isChangeable
     * If the coordinates are not correct, an exception will be thrown
     * @param x
     * @param y
     * @return true if the cell content can be changed in (x,y), false otherwise
     */
    bool isCellCanBeChanged(unsigned int x, unsigned int y) const;
    /**
     * @brief getSize returns the grid size
     * @return
     */
    int getSize() const;
    /**
     * @brief getDifficulty returns the grid difficulty
     * @return
     */
    Difficulty getDifficulty() const;
    /**
     * @brief updateGrid checks the correctness of the grid
     * It checks for errors in single cells (for example too much adjacents cells) or for rows and columns
     */
    void updateGrid();
    /**
     * @brief isAtLeastOneError checks if there is at least one error in the grid
     * @return true if there is at least one error, false otherwise
     */
    bool isAtLeastOneError();
    /**
     * @brief isGridFull checks if all the cell of the grid have their content different from empty cell
     * @return true if all cells contain a pawn, false otherwise
     */
    bool isGridFull();
    /**
     * @brief isNumberOfPawnsCorrectInGrid checks if the number of pawns in every row and column is correct
     * @return true if all row and columns are equals in term of number of pawns, false otherwise
     */
    bool isNumberOfPawnsCorrectInGrid();


private:
    std::vector<std::vector<Cell>> _grid;
    unsigned int _sizeGrid;
    Difficulty _difficulty;
    bool _isAtLeastOneErrorInGrid;
    /**
     * @brief getState is an inner method to get the state of a line.
     * The method is almost identical for a row or a column
     * @param index of the line
     * @param isRow is an argument to tell the method if the index is the index of a row or a column
     * @return
     */
    LineState getState(int index, bool isRow) const;
    /**
     * @brief checkIfInGrid checks the coordinate (x,y) is correct in the current grid
     * @param x
     * @param y
     * @return true if (x,y) exists in the current grid, false otherwise
     */
    bool checkIfInGrid(unsigned int x, unsigned int y) const;
    /**
     * @brief resetGridCorrectness resets all the errors previously declared in the grid
     */
    void resetGridCorrectness();
    /**
     * @brief cellHaveSameContentHorizontalNeighboor checks if a cell has adjacent neighboors with the same content in a row
     * Used to check if more than 2 pawns are adjacent. Not very useful if the cell at (x,y) is an empty cell
     * @param x
     * @param y
     * @return
     */
    bool cellHaveSameContentHorizontalNeighboor(int x, int y);
    /**
     * @brief cellHaveSameContentVerticalNeighboor checks if a cell has adjacent neighboors with the same content in a column
     * Used to check if more than 2 pawns are adjacent. Not very useful if the cell at (x,y) is an empty cell
     * @param x
     * @param y
     * @return
     */
    bool cellHaveSameContentVerticalNeighboor(int x, int y);
    /**
     * @brief check3SameCellValue checks if there is 3 or more pawns adjacent and if so, sets correctness of these cells
     * @see Cell::setCorrect
     */
    void check3SameCellValue();
    /**
     * @brief isColumnFull checks if a column is full e.g if all the cells of a column have a content different from empty cell
     * @param x The index of the column
     * @return true if all cells of the column x have a content different from empty cell
     */
    bool isColumnFull(int x);
    /**
     * @brief isRowFull checks if a row is full e.g if all the cells of a row have a content different from empty cell
     * @param x The index of the row
     * @return true if all cells of the row x have a content different from empty cell
     */
    bool isRowFull(int x);
    /**
     * @brief areRowsEquals checks if two rows, given by their indexes, are equals, meaning for all coordinates (y0,i) and (y1,i), the cell at (y0,i) has the same content as the cell at (y1,i)
     * @param y0 The index of the first row
     * @param y1 The index of the second row
     * @return true if the rows are equals, false otherwise
     */
    bool areRowsEquals(int y0, int y1);
    /**
     * @brief areColumnsEquals checks if two columns, given by their indexes, are equals, meaning for all coordinates (i,x0) and (i,x1), the cell at (i,x0) has the same content as the cell at (i,x1)
     * @param x0 The index of the first column
     * @param x1 The index of the second column
     * @return true if the columns are equals, false otherwise
     */
    bool areColumnsEquals(int x0, int x1);
    /**
     * @brief isNumberOfPawnsInLineCorrect is an inner method to check the number of pawns in a line
     * The method is almost identical for a row or a column
     * @param index of the line
     * @param isRow is an argument to tell the method if the index is the index of a row or a column
     * @return true if there is the same number of white and black pawns in the line, false otherwise
     */
    bool isNumberOfPawnsInLineCorrect(int index,bool isRow);
    /**
     * @brief isNumberOfPawnsInRowCorrect checks if the number of white and blacks pawns are equal in a row
     * @param rowIndex
     * @return true if there is the same number of white and black pawns in the row, false otherwise
     */
    bool isNumberOfPawnsInRowCorrect(int rowIndex);
    /**
     * @brief isNumberOfPawnsInColumnsCorrect checks if the number of white and blacks pawns are equal in a column
     * @param columnIndex
     * @return true if there is the same number of white and black pawns in the column, false otherwise
     */
    bool isNumberOfPawnsInColumnCorrect(int columnIndex);
    /**
     * @brief setColumnIncorrect sets all the cell of the column x as incorrect, meaning there is an error in the whole column
     * @param x The index of the column
     */
    void setColumnIncorrect(int x);
    /**
     * @brief setRowIncorrect sets all the cell of the row y as incorrect, meaning there is an error in the whole row
     * @param y The index of the row
     */
    void setRowIncorrect(int y);
    /**
     * @brief checkSameRowAndColumn checks if there is rows or columns identicals and if so, sets all their cells as incorrect
     */
    void checkSameRowAndColumn();
    /**
     * @brief setGridInError sets the state of the grid as "incorrect"
     */
    void setGridInError();
};

#endif // GRID_H
