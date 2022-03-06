/**
 * @file printInformation.h
 * @brief Header for the printing functions used for debugging
 * @author Jasmine Chaid-Akacem <jasmine.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#ifndef PROJET_EPIDEMIE_PRINTINFORMATIONS_H
#define PROJET_EPIDEMIE_PRINTINFORMATIONS_H
#include <string.h>
#include "citizenManager.h"
#include "define.h"

/**
 * @brief Print the town state. Show the state of each person in town based on stateCitizen
 * @param inhabitantsTab the list of inhabitants
 * @param sizeOfTown the array's size
 */
void printTownState(inhabitants *inhabitantsTab, int sizeOfTown);

#endif //PROJET_EPIDEMIE_PRINTINFORMATIONS_H
