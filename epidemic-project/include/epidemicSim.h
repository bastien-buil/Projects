/**
 * @file epidemicSim.h
 * @brief This file contains some of the structures used in the programs
 * @author Jasmine Chaid-Akacem <jasmine.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#ifndef PROJET_EPIDEMIE_EPIDEMICSIM_H
#define PROJET_EPIDEMIE_EPIDEMICSIM_H
#include "utils.h"
#include "citizenManager.h"

/**
 * Aliases of general-purposes numbers
 */
#define WIDTH 7
#define HEIGHT 7
#define POPULATION_NUMBER 35

/**
 * Aliases of buildings number for clarity
 */
#define NB_HOUSE 12
#define NB_HOSPITAL 1
#define NB_STATION 2

/**
 * Aliases of the number of each citizen's type for clarity
 */
#define NUMBER_OF_TYPE 4
#define REMAINING_JOURNALIST 2
#define REMAINING_DOCTOR 3
#define REMAINING_FIRE 4
#define REMAINING_CITIZEN 23

#define TURN_MAX 100

typedef struct simulation simulationInformation;
typedef struct statistics statistics;
typedef struct town town;

/**
 * Strcuture holding the information about the statistics of the simulation
 */
struct statistics {

    unsigned int numberOfContaminated;
    unsigned int numberOfDeath;
    unsigned int numberOfBurnt;

    float averageContaminationLevelOfTown;
};

/**
 * Structure representing the town, is at the heart of the simulation
 * Composed of the town [board], and its inhabitants [inhabitants]
 */
struct town {

    int nbInhabitantsAlive;

    boardCell board[WIDTH][HEIGHT];
    inhabitants inhabitant[POPULATION_NUMBER];
};

/**
 * Structure allowing to regroup every information on the simulation
 */
struct simulation {

    int currentTurn;

    town *_town;
    statistics *townStats;
};


/**
 * Used to increment the current number of turn when it receive a sigalrm from the timer
 * @param sig
 */
void startOfTurnEvent(__attribute__((unused)) int sig);

#endif //PROJET_EPIDEMIE_EPIDEMICSIM_H
