/**
* @file icondrawertype.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "icondrawertype.h"
#include "customiconsdrawer.h"
#include "roundicondrawer.h"



IconDrawer* IconDrawerType::getDrawerFromEnum(IconDrawerTypeEnum iconType){
    switch(iconType){
        case ROUND_ICONS:
            return new RoundIconDrawer();
        break;
        case GNU_TUX:
            return new CustomIconsDrawer(":/iconsFiles/icons/gnu.png", ":/iconsFiles/icons/tux.png");
        break;
        default:
            return nullptr;
    }
}
