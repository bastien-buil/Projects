/**
* @file roundicondrawer.h
* @author Bastien Buil
* @author J****** C**** A*****
**/

#ifndef ROUNDICONDRAWER_H
#define ROUNDICONDRAWER_H

#include "icondrawer.h"

/**
 * @brief The RoundIconDrawer class draws a simple circle filled with the appropriate color as a pawn
 */
class RoundIconDrawer : public IconDrawer
{
public:
    RoundIconDrawer();
    /**
     * @brief draw draws the icon on the screen @see IconDrawer::draw
     * @param p
     * @param x
     * @param y
     * @param size
     * @param c
     * @param selected
     */
    void draw(QPainter& p, int x, int y, int size, Cell c, bool selected);
    ~RoundIconDrawer();
};

#endif // ROUNDICONDRAWER_H
