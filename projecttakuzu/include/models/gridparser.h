/**
* @file gridparser.h
* @author Bastien Buil
* @author J****** C**** A*****
**/
#ifndef GRIDPARSER_H
#define GRIDPARSER_H

#include "grid.h"
#include <QString>
/**
 * @brief The GridParser class creates a grid from a string properly formated
 * The string should be row first, and each content formated according to a standard @see getCellContentFromChar
 * This class is not meant to be instanciated
 * Here, for ease reasons, QString is used but it should not be the case normally
 */
class GridParser
{
public:
    /**
     * @brief buildGridFromString builds a grid with a string properly formated
     * @param sizeGrid is the size of the future grid. The string provided should be exactly of size sizeGrid*sizeGrid. If less, the program will crash
     * @param difficulty is the difficulty of the future grid, according to the string provided
     * @param gridInStringFormat the string representing the grid
     * @return a new Grid initialized according to the string representation
     */
    static Grid buildGridFromString(int sizeGrid,Difficulty difficulty,QString gridInStringFormat);
private:
    /**
     * @brief getCellContentFromChar gives the cell content represented by the char element
     * "." is empty cell
     * "B" is black pawn
     * "W" is white pawn
     * Otherwise it will thrown an exception
     * @param element
     * @return The correct cell content according to the char element
     */
    static CELL_CONTENT getCellContentFromChar(QChar element);
};

#endif // GRIDPARSER_H
