/**
 * @file doctorUtils.h
 * @brief This file contains function related to the job of doctor
 * @author Jasmine Chaid-Akacem <jasmine.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#ifndef PROJET_EPIDEMIE_DOCTORUTILS_H
#define PROJET_EPIDEMIE_DOCTORUTILS_H
#include "citizenManager.h"

/**
 * @brief check if the individual is alive
 * @param individual the individual to check
 * @return 1 if alive
 */
int notAfflicted(inhabitants *individual);

/**
 * @brief Check if a doctor can heal themself
 * @param doctor the doctor to check
 * @return 1 if they can heal themself
 */
int canSelfHeal(inhabitants *doctor);

/**
 * @brief Check if an individual can heal someone else (in practice is always a doctor)
 * @param individual the individual to check
 * @return 1 if they are capable to do so
 */
int canHealPeople(inhabitants *individual);

/**
 * @brief Check if the condition for the number of doses to decrease are met
 * @param cellType the type of cell in which the doctor is
 * @param nbDoses  the current number of doses
 * @param healed   a boolean indicating if the doctor as healed someone
 * @return 1 if such conditions are met
 */
int allowedForNumberOfDosesToDecrease(int cellType, unsigned int nbDoses, unsigned int healed);

/**
 * @brief Heal someone by changing their status back to alive
 * @param individual the individual to heal
 */
void heal(inhabitants *individual);

/**
 * @brief used for qsort, this function establish a partial order based on the sickness and contamination
 * a.k.a Sick ++ > Sick + > Alive ++ > Alive + ; where "+" indicates the level of contamination
 * @param p1 the first individual
 * @param p2 the second individual
 * @return 0 if said individuals are equals, -1 if p1 is inferior, 1 if p1 is superior
 */
int compareContaminationAndSickness(const void *p1, const void *p2);

/**
 * @brief Construct a sub-Array of the array of individuals based on the people in a certain cell
 * @param originArray the array to extract individuals from
 * @param subSetCell the sub-array
 * @param currentCell the cell indicating which people to extract
 */
void selectiveCopy(inhabitants *originArray, inhabitants **subSetCell, boardCell *currentCell);

#endif //PROJET_EPIDEMIE_DOCTORUTILS_H
