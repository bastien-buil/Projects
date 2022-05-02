/**
* @file icondrawer.h
* @author Bastien Buil
* @author J****** C**** A*****
**/

#ifndef ICONDRAWER_H
#define ICONDRAWER_H

#include <QPainter>
#include "cell.h"
/**
 * @brief The IconDrawer class represents an icon for a pawn
 */
class IconDrawer
{
public:
    /**
     * @brief draw draws the icon at (x,y) on the screen. This icon represents the Cell c
     * @param p
     * @param x coordinate on the screen
     * @param y coordinate on the screen
     * @param size of the icon on the screen
     * @param c Cell drawn
     * @param selected tells the method if the player has his mouse above the icon
     */
    virtual void draw(QPainter& p, int x, int y, int size, Cell c, bool selected) = 0;
    virtual ~IconDrawer(){};

};


#endif // ICONDRAWER_H
