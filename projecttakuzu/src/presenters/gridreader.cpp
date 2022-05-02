/**
* @file gridreader.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/
#include "gridreader.h"

#include <QIODevice>
#include <QRandomGenerator>
#include <stdexcept>
#include <iostream>

#include "gridparser.h"

using namespace std;

Grid GridReader::getGridFromFile(int sizeGrid, Difficulty difficulty) {

    QString filename = getFilename(sizeGrid,difficulty);

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        throw invalid_argument("No gridFile with sizeGrid and difficulty provided");
    }

    QTextStream in(&file);

    int numberOfGrids = getNumberOfGrids(in);

    QString gridInLineString = selectRandomGridFromFile(in,numberOfGrids);

    file.close();

    if(gridInLineString.size() != sizeGrid*sizeGrid) {

        throw invalid_argument("Invalid format for gridLine found");
    }

    return GridParser::buildGridFromString(sizeGrid,difficulty,gridInLineString);

}

QString GridReader::getFilename(int sizeGrid, Difficulty difficulty) {

    QString filename(":/gridsFiles/grids/"+QString::number(sizeGrid)+"_");

    switch (difficulty) {
        case EASY:
            filename = filename.append(QString("easy"));
            break;
        case HARD:
            filename = filename.append(QString("hard"));
            break;

    }
    filename = filename.append(QString(".txt"));

    return filename;

}

int GridReader::getNumberOfGrids(QTextStream &streamInputFile) {

    if(streamInputFile.atEnd()) {
        throw invalid_argument("Invalid format for gridfile");
    }

    QString numberOfGridsRead = streamInputFile.readLine();

    int numberOfGrids = numberOfGridsRead.toInt();

    //if numberOfGrid == 0, it's invalid, even if the number 0 was actually in the file

    if(numberOfGrids == 0) {

        throw invalid_argument("Invalid format for gridfile");
    }

    return numberOfGrids;


}

QString GridReader::selectRandomGridFromFile(QTextStream &streamInputFile, int numberOfGrids) {

    int selectedGrid = QRandomGenerator::global()->bounded(numberOfGrids);

    for(int i=0;i<selectedGrid && !streamInputFile.atEnd();i++) {

        streamInputFile.readLine();
    }

    if(streamInputFile.atEnd()) {

        throw invalid_argument("Invalid format for gridfile");
    }

    return streamInputFile.readLine();

}
