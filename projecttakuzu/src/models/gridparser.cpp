/**
* @file gridparser.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "gridparser.h"

#include <stdexcept>

using namespace std;

Grid GridParser::buildGridFromString(int sizeGrid,Difficulty difficulty, QString gridInStringFormat) {

    Grid grid(sizeGrid,difficulty);

    for(int i=0;i<sizeGrid;i++) {

        for(int j=0;j<sizeGrid;j++) {

            QChar element(gridInStringFormat.at(i*sizeGrid+j));

            CELL_CONTENT content = getCellContentFromChar(element);

            if(content != EMPTY_CELL) {
                grid.setCellContent(i,j,content);
                grid.setCellDefinitive(i,j);
            }


        }
    }

    return grid;
}


CELL_CONTENT GridParser::getCellContentFromChar(QChar element) {

    char character = element.toLatin1();

    switch (character) {
        case '.':
            return EMPTY_CELL;
        case 'B':
            return BLACK;
        case 'W':
            return WHITE;
        default:
            throw invalid_argument("Invalid character element provided");
    }


}
