/**
* @file roundicondrawer.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "roundicondrawer.h"

RoundIconDrawer::RoundIconDrawer()
{



}

void RoundIconDrawer::draw(QPainter& p, int x, int y, int size, Cell c, bool selected){
    int padding = size / 10;
    int circleSize = size - 2 * padding;

    if(c.getContent() != EMPTY_CELL){
        QBrush brush(Qt::blue);
        if(c.getContent() == WHITE){
            brush = Qt::white;
        }else if(c.getContent() == BLACK){
            brush = Qt::black;
        }
        p.setBrush(brush);
        QPen pen((!c.isCorrect()) ? Qt::GlobalColor::red : Qt::black, size/20);
        p.setPen(pen);
        p.drawEllipse(x + padding, y + padding, circleSize, circleSize);

        if(selected){
            QPen pen2(selected ? Qt::GlobalColor::magenta : Qt::black, size/40);
            p.setPen(pen2);
            p.drawEllipse(x + padding, y + padding, circleSize, circleSize);
        }

        if(!c.isChangeable()){
            p.setBrush(c.getContent() == BLACK ? Qt::white : Qt::black);
            p.setPen(c.getContent() == BLACK ? Qt::white : Qt::black);
            p.drawRect(x + size * 2 / 5, y + size * 2 / 5, size * 1/5, size * 1/5);
        }
    }



}

RoundIconDrawer::~RoundIconDrawer(){}
