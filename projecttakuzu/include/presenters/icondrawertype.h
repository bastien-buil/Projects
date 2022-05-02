/**
* @file icondrawertype.h
* @author Bastien Buil
* @author J****** C**** A*****
**/

#ifndef ICONDRAWERTYPE_H
#define ICONDRAWERTYPE_H

#include "icondrawer.h"

namespace IconDrawerType
{
    /**
     * @brief The IconDrawerTypeEnum enum defines the icons currently supported by the game
     */
    enum IconDrawerTypeEnum {ROUND_ICONS, GNU_TUX};
    /**
     * @brief getDrawerFromEnum returns the appropriate icon drawer object, given the argument @see IconDrawer
     * @param iconType
     * @return
     */
    IconDrawer* getDrawerFromEnum(IconDrawerTypeEnum iconType);
};

#endif // ICONDRAWERTYPE_H
