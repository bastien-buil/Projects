/**
* @file customiconsdrawer.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "customiconsdrawer.h"

CustomIconsDrawer::CustomIconsDrawer(QString blackImage, QString whiteImage)
{

    _blackIcon = QImage(blackImage);
    _whiteIcon = QImage(whiteImage);

}


void CustomIconsDrawer::draw(QPainter& p, int x, int y, int size, Cell c, bool selected){
    int padding = size / 10;
    int circlePadding = size / 15;
    int circleSize = size - 2 * circlePadding;
    int crossPadding = size / 8;

    QRect crossRect(x + crossPadding, y + crossPadding, size - 2 * crossPadding, size - 2 * crossPadding);

    if(c.getContent() != EMPTY_CELL){
        QImage icon;
        QRect imageRect(x + padding, y + padding, size - 2 * padding, size - 2 * padding);
        if(c.getContent() == WHITE){
            icon = _whiteIcon;
        }else if(c.getContent() == BLACK){
            icon = _blackIcon;
        }

        if(!c.isChangeable()){
            p.setBrush(Qt::darkCyan);
            p.setPen(Qt::darkCyan);
            p.drawEllipse(x + circlePadding, y + circlePadding, circleSize, circleSize);
        }

        if(selected){
            QPen pen2(selected ? Qt::GlobalColor::magenta : Qt::black, size/40);
            p.setBrush(Qt::transparent);
            p.setPen(pen2);
            p.drawEllipse(x + circlePadding, y + circlePadding, circleSize, circleSize);
        }

        p.drawImage(imageRect, icon);


        if(!c.isCorrect()){
            p.setBrush(Qt::red);
            QPen pen(Qt::red, size/20);
            p.setPen(pen);
            p.drawLine(crossRect.x(), crossRect.y(), crossRect.x() + crossRect.width(), crossRect.y() + crossRect.height());
            p.drawLine(crossRect.x() + crossRect.width(), crossRect.y(), crossRect.x(), crossRect.y() + crossRect.height());
        }


    }



}

CustomIconsDrawer::~CustomIconsDrawer(){}
