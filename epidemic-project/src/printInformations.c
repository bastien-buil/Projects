/**
 * @file printInformation.c
 * @brief File reuniting the function to print various information
 * @author Jasmine Chaid-Akacem <jasmin.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#include "printInformations.h"

/**
 * @brief Get the state of an individual. The format used is {a, s, d, b}{c, f, j, d}index as explained in the README.
 * The letter is the first letter of their state and their job
 * @param individual the individual on which we want to know the state
 * @param desc an array of char that will contain this information
 * @return a string of the state
 */
char* stateCitizen(inhabitants individual, char desc[]) {

    switch (individual.condition) {
        case alive:
            strcpy(desc, "a");
            break;
        case sick:
            strcpy(desc, "s");
            break;
        case dead:
            strcpy(desc, "d");
            break;
        case burned:
            strcpy(desc, "b");
            break;
        default:
            break;
    }

    switch (individual.job) {
        case citizen:
            strcat(desc, "c");
            break;
        case doctor:
            strcat(desc, "d");
            break;
        case fireFighter:
            strcat(desc, "f");
            break;
        case journalist:
            strcat(desc, "j");
            break;
    }

    return desc;
}

void printTownState(inhabitants *inhabitantsTab, int sizeOfTown) {

    int i;
    char desc[3] = "  ";
    fprintf(stderr, "[");
    for (i=0; i < sizeOfTown-1; i++) {
        fprintf(stderr, "%s%d, ", stateCitizen(inhabitantsTab[i], desc), i);
        desc[0] = '\0';
    }
    fprintf(stderr, "%s%d]\n", stateCitizen(inhabitantsTab[sizeOfTown-1], desc), sizeOfTown-1);
}
