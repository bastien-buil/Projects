/**
* @file customiconsdrawer.h
* @author Bastien Buil
* @author J****** C**** A*****
**/
#ifndef CUSTOMICONSDRAWER_H
#define CUSTOMICONSDRAWER_H


#include "icondrawer.h"

/**
 * @brief The CustomIconsDrawer class represents a custom icon, drawn from an image
 */
class CustomIconsDrawer : public IconDrawer
{
public:
    /**
     * @brief CustomIconsDrawer constructor
     * @param blackImage The image representing the black pawn in our convention
     * @param whiteImage The image representing the white pawn in our convention
     */
    CustomIconsDrawer(QString blackImage, QString whiteImage);
    /**
     * @brief draw draws the icon on the view @see IconDrawer::draw
     * @param p
     * @param x
     * @param y
     * @param size
     * @param c
     * @param selected
     */
    void draw(QPainter& p, int x, int y, int size, Cell c, bool selected);
    ~CustomIconsDrawer();
private:
    QImage _blackIcon;
    QImage _whiteIcon;
};

#endif // CUSTOMICONSDRAWER_H
