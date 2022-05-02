/**
* @file gridreader.h
* @author Bastien Buil
* @author J****** C**** A*****
**/
#ifndef GRIDREADER_H
#define GRIDREADER_H

#include "grid.h"
#include <QFile>
#include <QTextStream>

/**
 * @brief The GridReader class chooses a random grid from a file and extract a string representation of the grid. Then, a Grid object can be created.
 * This class is not meant to be instanciated
 */
class GridReader
{
public:
    /**
     * @brief getGridFromFile creates a grid object with a string representation found in a file
     * The file name depends on the grid size and the difficulty chosen
     * The grid is chosen randomly
     * This method can thrown exceptions for various reasons: grid file not found, grid file poorly formated
     * @param sizeGrid
     * @param difficulty
     * @return a new random Grid
     */
    static Grid getGridFromFile(int sizeGrid,Difficulty difficulty);
private:
    /**
     * @brief getFilename returns the standard filename for a grid file, given the grid size and the difficulty
     * The filename is constructed prefixed with the ressource prefix. Do not move the grid files.
     * @param sizeGrid
     * @param difficulty
     * @return The constructed filename
     */
    static QString getFilename(int sizeGrid, Difficulty difficulty);
    /**
     * @brief getNumberOfGrids reads the number of grids inside the file provided through a QTextStream
     * The number of grid is specified at the beginning of the file (the first line)
     * @param streamInputFile The grid file
     * @return
     */
    static int getNumberOfGrids(QTextStream &streamInputFile);
    /**
     * @brief selectRandomGridFromFile selects a random string representation of a grid and returns it
     * The random number is chosen with a QRandomGenerator
     * @param streamInputFile The grid file
     * @param numberOfGrids
     * @return The string representation of the grid selected
     */
    static QString selectRandomGridFromFile(QTextStream &streamInputFile, int numberOfGrids);
};

#endif // GRIDREADER_H
