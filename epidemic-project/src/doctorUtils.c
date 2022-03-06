/**
 * @file doctorUtils.c
 * @brief File with usefull fonction to manage the doctors actions
 * @author Jasmine Chaid-Akacem <jasmin.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#include "doctorUtils.h"

int notAfflicted(inhabitants *individual) {
    return individual->condition == alive;
}

int canSelfHeal(inhabitants *doctor) {
    return (doctor->condition == sick) && (doctor->timeElapsedSinceSick < 10) && (doctor->numberOfDoses > 0);
}

int canHealPeople(inhabitants *individual) {
    return notAfflicted(individual) && (individual->numberOfDoses > 0) && (!individual->hasHealed) && (individual->job == doctor);
}

int allowedForNumberOfDosesToDecrease(int cellType, unsigned int nbDoses, unsigned int healed) {
    return (nbDoses > 0) && (cellType != hospital) && (healed == 1);
}

void heal(inhabitants *individual) {

    if (individual->condition == sick) {
        individual->condition = alive;
        individual->timeElapsedSinceSick = 0;
    }
}

int compareContaminationAndSickness(const void *p1, const void *p2){

    inhabitants *i1 = *(inhabitants**)p1;
    inhabitants *i2 = *(inhabitants**)p2;
    float c1 = (i1)->contaminationLevel;
    float c2 = (i2)->contaminationLevel;
    int s1 = (i1)->condition;
    int s2 = (i2)->condition;

    if ((c1 == c2)&&(s1 == s2)) return 0;
    else if ((c1 < c2)&&(s1==s2)) return -1;
    else if ((c1 > c2)&&(s1==s2)) return 1;

    else if ((c1 <= c2)&&(s1==alive && s2 ==sick)) return -1;
    else if ((c1 <= c2)&&(s1==sick && s2 ==alive)) return 1;
    else if ((c1 > c2)&&(s1==alive && s2 ==sick)) return -1;
    else if ((c1 > c2)&&(s1==sick && s2 ==alive)) return 1;

    return 1;
}

void selectiveCopy(inhabitants *originArray, inhabitants**subSetCell, boardCell *currentCell) {
    int i;
    for (i=0; i < currentCell->numberOfOccupants; i++) {
        subSetCell[i] = &originArray[currentCell->residents[i]];
    }
}

