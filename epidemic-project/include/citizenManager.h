/**
 * @file citizenManager.h
 * @brief This file contains many of the structure used in the programs. This header contains to of the most important
 * functions of this program which are the creation of each thread and their action when created
 * @author Jasmine Chaid-Akacem <jasmine.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#ifndef PROJET_EPIDEMIE_CITIZENMANAGER_H
#define PROJET_EPIDEMIE_CITIZENMANAGER_H

#define MAX_CAPACITY 16
#define NEIGHBOURS_NUMBER 8
#define MAX_LENGTH_MESSAGE_FOR_INT 3
#define MAX_LENGTH_MESSAGE_FOR_FLOAT 318

typedef struct _inhabitants inhabitants;
typedef struct _boardCell boardCell;

typedef enum cellType {
    wasteland,
    hospital,
    station,
    house
} cellType;

typedef enum jobs {
    citizen,
    doctor,
    fireFighter,
    journalist
} jobs;

typedef enum status {
    alive,
    sick,
    dead,
    burned
} status;

struct _inhabitants {

    jobs job;
    status condition;

    unsigned int timeElapsedSinceSick;
    unsigned int timeElapsedSinceCuredInHospital;
    unsigned int timeOnSameCase;
    unsigned int numberOfDoses;
    unsigned int hasHealed;
    unsigned int timeElapsedSinceOutOfHospital;
    float remainingSpray;

    int xPosition;
    int yPosition;

    float contaminationLevel;
};

struct _boardCell {

    cellType type;

    int xPosition;
    int yPosition;
    int capacity;
    int numberOfOccupants;

    float contaminationLevel;

    int residents[MAX_CAPACITY];
};

/**
  * @brief Used by each tread during its execution. It uses one of the job's action function depending the the
  * inhabitant job. If the inhabitant is dead or burnt, it only uses the contamination propagation method
  * @param parameters the inhabitant index
  * @return NULL
  */
void *action(void *parameters);

/**
  * @brief create a thread for each inhabitant of the city
  */
void createThreadForEachInhabitant();


#endif //PROJET_EPIDEMIE_CITIZENMANAGER_H
