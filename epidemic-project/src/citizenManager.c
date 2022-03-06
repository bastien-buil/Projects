/**
 * @file citizenManager.c
 * @brief File with all the function to create and manage the inhabitant thread
 * @author Jasmine Chaid-Akacem <jasmin.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#include <time.h>
#include <pthread.h>
#include <search.h>
#include "epidemicSim.h" // below include citizenManager.h
#include "doctorUtils.h"
#include "printInformations.h"

#ifdef __APPLE__
#include "pthread_barrier.h"
#endif

volatile sig_atomic_t roundFlag = 0;
volatile sig_atomic_t endGame = 0;

int fdCity;
int fdStats;

town *citySharedMemory;
inhabitants *inhabitantsOfCity;
statistics *statsSharedMemory;

sem_t *citySem;
sem_t *statsSem;
mqd_t pressChannel;

pthread_barrier_t barrier;
pthread_mutex_t channelMutex;
pthread_mutex_t citizenMutex;

/**
  * @brief Get the structure city which was sent in the shared memory by epidemicSim. Open the city semaphore
  */
void getCity() {

    fdCity = shm_open(MEMTOWN,O_RDWR,S_IRUSR | S_IWUSR);

    if(fdCity == -1) {
        fprintf(stderr,"Erreur ouverture mémoire partagée %s\nCitizenManager va quitter\n",MEMTOWN);
        exit(EXIT_FAILURE);
    }

    citySharedMemory = mmap(NULL,sizeof(town),PROT_READ|PROT_WRITE,MAP_SHARED,fdCity,0);

    if(citySharedMemory == NULL) {
        fprintf(stderr, "Erreur projection mémoire partagée %s\nCitizenManager va quitter\n", MEMTOWN);
        exit(EXIT_FAILURE);
    }

    citySem = sem_open(SEMTOWN,0);

    if(citySem == SEM_FAILED) {
        fprintf(stderr,"Erreur ouverture semaphore %s\nCitizenManager va quitter\n",MEMTOWN);
    }
}

/**
  * @brief Get the inhabitant's array contained in the city
  */
void getInhabitants() {
    inhabitantsOfCity = citySharedMemory->inhabitant;
}

/**
  * @brief Get the structure statistics from the shared memory
  */
void getStats() {

    fdStats = shm_open(MEMSTATS,O_RDWR,S_IRUSR|S_IWUSR);

    if(fdStats == -1) {
        fprintf(stderr,"Erreur ouverture mémoire partagée %s\nCitizenManager va quitter\n",MEMSTATS);
        exit(EXIT_FAILURE);
    }

    statsSharedMemory = mmap(NULL,sizeof(statistics),PROT_READ|PROT_WRITE,MAP_SHARED,fdStats,0);

    if(statsSharedMemory == NULL) {
        fprintf(stderr, "Erreur projection mémoire partagée %s\nCitizenManager va quitter\n", MEMSTATS);
        exit(EXIT_FAILURE);
    }

    statsSem = sem_open(SEMSTATS,0);

    if(statsSem == SEM_FAILED) {
        fprintf(stderr,"Erreur ouverture semaphore %s\nCitizenManager va quitter\n",MEMTOWN);
    }

}

/**
  * @brief Return a boolean which will be used to know if the inhabitant state has to be changed to sick
  * @param individual the concerned inhabitant
  * @param isSick a random double between 0 and 1
  * @return 1 if the random number is inferior than the contamination level of
  * the inhabitant and if the inhabitant isn't already sick
  */
int willBeSick(inhabitants *individual, double isSick) {
    int canBeInSickState = notAfflicted(individual);
    return (isSick < individual->contaminationLevel) && canBeInSickState;
}

/**
  * @brief Return a boolean indicating if the possibly infected person will contaminate his neighbours
  * @param probability the probability that the person can infect his neighbours
  * @param randomDouble a random double between 0 and 1
  * @param possibleInfected a possibly infected inhabitant
  * @return 1 if the person is dead or sick and the randomDouble inferior than
  * the probability, 0 else
  */
int willContaminationPropagate(double probability, double randomDouble, inhabitants *possibleInfected) {
    return randomDouble < probability && (possibleInfected->condition == sick || possibleInfected->condition == dead);
}

/**
  * @brief Return a boolean indicating if a person is dead or burnt
  * @param individual the person whose condition is to be verified
  * @return 1 if the individual is burnt or dead, 0 else
  */
int notDeadOrBurnt(inhabitants *individual) {
    return individual->condition != dead && individual->condition != burned;
}

/**
  * @brief Return a boolean indicating if a person is dead or burnt
  * @param individual the person whose condition is to be verified
  * @return 0 if the individual is burnt or dead, 1 else
  */
int isDeadOrBurnt(inhabitants *inhabitant) {
    return !notDeadOrBurnt(inhabitant);
}

/**
  * @brief Return a boolean indicating if a cell position is contained in the city
  * @param x first coordinate of the cell in the city matrix
  * @param y second coordinate of the cell in the city matrix
  * @return 1 if the cell is in the city, 0 else
  */
int isInBorder(int x, int y) {
    return (x>0 && y>0) && (x*WIDTH + y < WIDTH*HEIGHT);
}

/**
  * @brief Raise the contamination level of all the inhabitants in the current
  * cell except for the source of the contamination
  * @param currentCell the infected cell
  * @param sourceOfInfection the person who is infecting the others in its cell
  * can be NULL, in which case everyone present in the cell will be infected (used for the propagation to different cell)
  */
void infectNeighbours(boardCell *currentCell, inhabitants *sourceOfInfection, unsigned int *generatorStatePropagation) {

    int i;
    int index;
    int nbOccupants;
    nbOccupants = currentCell->numberOfOccupants;

    for (i=0; i < nbOccupants; i++) {

        sem_wait(citySem);
        index = currentCell->residents[i];
        sem_post(citySem);

        pthread_mutex_lock(&citizenMutex);

        if (&citySharedMemory->inhabitant[index] != sourceOfInfection) {
            if(citySharedMemory->inhabitant[index].job == fireFighter){
                double isPropagatingOnFirefighter = (double)rand_r(generatorStatePropagation)/(double)RAND_MAX;
                if(isPropagatingOnFirefighter <= 0.7){
                    pthread_mutex_unlock(&citizenMutex);
                    continue;
                }
            }
            (&citySharedMemory->inhabitant[index])->contaminationLevel += (float)0.01;
        }
        pthread_mutex_unlock(&citizenMutex);
    }
}


/**
  * @brief The cell in the position x,y raise the level of contamination of all
  * the inhabitant of the neighbour wastelands
  * @param x first coordinate of the current cell in the city matrix
  * @param y second coordinate of the current cell in the city matrix
  */
void infectNeighboursCell(int x, int y, unsigned int *generatorStatePropagation) {

    int i;
    int newX;
    int newY;
    int dx[NEIGHBOURS_NUMBER] = {1, 1, 0, -1, -1, -1, 0, 1};
    int dy[NEIGHBOURS_NUMBER] = {0, -1, -1, -1, 0, 1, 1, 1};

    boardCell *aroundCell;
    for (i=0; i < NEIGHBOURS_NUMBER; i++) {

        newX = x+dx[i];
        newY = y+dy[i];
        if (isInBorder(newX, newY)) {

            sem_wait(citySem);
            aroundCell = &citySharedMemory->board[newX][newY];
            sem_post(citySem);

            if (aroundCell->type == wasteland) {
                infectNeighbours(aroundCell, NULL, generatorStatePropagation);
            }
        }
    }
}

/**
  * @brief Action of the possibly infected citizen (they can contaminate their
  * neighbours or the wasteland close to them)
  * @param possibleInfected a possibly infected person
  * @param generatorStatePropagation a seed for the generator of random numbers
  */
int infectedAction(inhabitants *possibleInfected, unsigned int generatorStatePropagation) {

    double isPropagating = (double)rand_r(&generatorStatePropagation)/(double)RAND_MAX;
    double isPropagatingAround = (double)rand_r(&generatorStatePropagation)/(double)RAND_MAX;

    sem_wait(citySem);
    boardCell *currentCell = &citySharedMemory->board[possibleInfected->xPosition][possibleInfected->yPosition];
    sem_post(citySem);

    if (willContaminationPropagate(0.1, isPropagating, possibleInfected)) {
        infectNeighbours(currentCell, possibleInfected, &generatorStatePropagation);
    }

    if (willContaminationPropagate(0.01, isPropagatingAround, possibleInfected) && currentCell->type == wasteland) {
        infectNeighboursCell(possibleInfected->xPosition, possibleInfected->yPosition, &generatorStatePropagation);
    }

    return 0;
}
/**
  * @brief Look for dead bodies in the cell of position x,y and change their state to burnt
  * @param x first coordinate of the cell
  * @param y second coordinate of the cell
  */
void burnDeadBodiesInCell(int x, int y){

    boardCell currentCell = citySharedMemory->board[x][y];
    for (int i = 0; i < currentCell.numberOfOccupants; i++) {
        int indexOfInhabitant = currentCell.residents[i];
        if (inhabitantsOfCity[indexOfInhabitant].condition == dead) {
            inhabitantsOfCity[indexOfInhabitant].condition = burned;
            sem_wait(statsSem);
            statsSharedMemory->numberOfBurnt++;
            sem_post(statsSem);
        }
    }

}

/**
  * @brief Send through the pressChannel the average contamination level of the city,
  * the number of death, the number of contaminated and the contamination level of the current journalist
  * @param inhabitant the journalist who is acting
  */
void JournalistAction(inhabitants *inhabitant) {

    int myContaminationLevel;
    float averageContaminationLevel;
    unsigned int numberOfDeath;
    unsigned int numberOfContaminated;

    char messageForMyContaminationLevel[MAX_LENGTH_MESSAGE_FOR_INT];
    char messageForMedianContaminationLevel[MAX_LENGTH_MESSAGE_FOR_FLOAT];
    char messageForNumberOfDeath[MAX_LENGTH_MESSAGE_FOR_INT];
    char messageForNumberOfContaminated[MAX_LENGTH_MESSAGE_FOR_INT];

    myContaminationLevel = (int)(inhabitant->contaminationLevel*100);
    sem_wait(statsSem);
    averageContaminationLevel = statsSharedMemory->averageContaminationLevelOfTown*100;
    numberOfDeath = statsSharedMemory->numberOfDeath;
    numberOfContaminated = statsSharedMemory->numberOfContaminated;
    sem_post(statsSem);

    int lengthContaminationMessage = snprintf(messageForMyContaminationLevel, MAX_LENGTH_MESSAGE_FOR_INT, "%d", myContaminationLevel);
    int lengthMedianContaminationMessage = snprintf(messageForMedianContaminationLevel, MAX_LENGTH_MESSAGE_FOR_FLOAT, "%f", averageContaminationLevel);
    int lengthDeathMessage = snprintf(messageForNumberOfDeath, MAX_LENGTH_MESSAGE_FOR_INT, "%d", numberOfDeath);
    int lengthContaminatedMessage = snprintf(messageForNumberOfContaminated, MAX_LENGTH_MESSAGE_FOR_INT, "%d", numberOfContaminated);

    pthread_mutex_lock(&channelMutex);
    mq_send(pressChannel, messageForMyContaminationLevel, lengthContaminationMessage, JOURNALIST_CONTAMINATION);
    pthread_mutex_unlock(&channelMutex);

    pthread_mutex_lock(&channelMutex);
    mq_send(pressChannel, messageForMedianContaminationLevel, lengthMedianContaminationMessage, CITY_CONTAMINATION);
    pthread_mutex_unlock(&channelMutex);

    pthread_mutex_lock(&channelMutex);
    mq_send(pressChannel, messageForNumberOfDeath, lengthDeathMessage, DEATH_TOLL);
    pthread_mutex_unlock(&channelMutex);

    pthread_mutex_lock(&channelMutex);
    mq_send(pressChannel, messageForNumberOfContaminated, lengthContaminatedMessage, CONTAMINATION_TOLL);
    pthread_mutex_unlock(&channelMutex);
}

/**
  * @brief A doctor will heal themself in priority, otherwise heal someone if possible
  * @param doctor the doctor who is acting
  */
int doctorAction(inhabitants *doctor) {

    inhabitants *mostSickPerson;
    inhabitants **inhabitantsTab;

    if (canSelfHeal(doctor)) {
        doctor->hasHealed = 1;
        heal(doctor);

        sem_wait(statsSem);
        statsSharedMemory->numberOfContaminated--;
        sem_post(statsSem);

    }

    sem_wait(citySem);
    boardCell *currentCell = &citySharedMemory->board[doctor->xPosition][doctor->yPosition];
    sem_post(citySem);

    inhabitantsTab = malloc(sizeof(inhabitants*)*currentCell->numberOfOccupants);

    if (inhabitantsTab == NULL) {
        perror("could not malloc");
        exit(EXIT_FAILURE);
    }

    if (canHealPeople(doctor)) {
        selectiveCopy(inhabitantsOfCity, inhabitantsTab, currentCell);
        qsort(inhabitantsTab, currentCell->numberOfOccupants, sizeof(inhabitants*), compareContaminationAndSickness);
        mostSickPerson = inhabitantsTab[currentCell->numberOfOccupants-1];

        if(mostSickPerson->condition == sick) {
            doctor->hasHealed = 1;
            heal(mostSickPerson);
            sem_wait(statsSem);
            statsSharedMemory->numberOfContaminated--;
            sem_post(statsSem);
        }
    }

    if(allowedForNumberOfDosesToDecrease(currentCell->type, doctor->numberOfDoses, doctor->hasHealed)) {
        doctor->numberOfDoses--;
    }

    free(inhabitantsTab);
    return 0;
}

/**
  * @brief If the firefighter has enough spray he decontaminates the people in the cell.
  * If he still has spray he decontaminate the cell.
  * @param firefighter the firefighter acting
  */
void firefighterAction(inhabitants* firefighter) {

    burnDeadBodiesInCell(firefighter->xPosition,firefighter->yPosition);

    float availableSpray = firefighter->remainingSpray / 10;
    boardCell* currentCell = &citySharedMemory->board[firefighter->xPosition][firefighter->yPosition];

    for (int i=0; i < currentCell->numberOfOccupants; i++) {
        int residentId = currentCell->residents[i];
        inhabitants* inhabitant = &citySharedMemory->inhabitant[residentId];
        if(availableSpray > 0 && inhabitant->contaminationLevel > 0){
            pthread_mutex_lock(&citizenMutex);
            float usedSprayOnCitizen = 0.2f;
            if(usedSprayOnCitizen > availableSpray) {
                pthread_mutex_unlock(&citizenMutex);
                continue;
            }

            availableSpray -= usedSprayOnCitizen;
            inhabitant->contaminationLevel -= usedSprayOnCitizen*inhabitant->contaminationLevel;
            if(inhabitant->contaminationLevel < 0){
                inhabitant->contaminationLevel = 0.0f;
            }
            pthread_mutex_unlock(&citizenMutex);
        }
    }

    sem_wait(citySem);
    if(availableSpray > 0 && currentCell->contaminationLevel > 0){
        float usedSprayOnCell = 0.2f;
        if(usedSprayOnCell <= availableSpray) {
            availableSpray -= usedSprayOnCell;
            currentCell->contaminationLevel -= usedSprayOnCell*currentCell->contaminationLevel;
            if (currentCell->contaminationLevel < 0) {
                currentCell->contaminationLevel = 0.0f;
            }
        }
    }
    sem_post(citySem);

    float usedSpray = firefighter->remainingSpray / 10 - availableSpray;
    firefighter->remainingSpray -= usedSpray;
}

/**
  * @brief Return a boolean indicating if the inhabitant can enter in the hospital
  * @param inhabitant the current inhabitant
  * @return 1 if the inhabitant is sick, is a firefighter or is a doctor who
  * left the hospital more than 2 days ago, 0 else
  */
int canEnterInHospital(inhabitants *inhabitant) {

    if(inhabitant->condition == sick) {
        return 1;
    }

    return inhabitant->job == fireFighter || (inhabitant->job == doctor && inhabitant->timeElapsedSinceOutOfHospital > 2);
}

/**
  * @brief Return a boolean indicating if the inhabitant has to leave the hospital
  * @param inhabitant current inhabitant
  * @return 1 if the person isn't a doctor, isn't sick and is in the hospital
  * for more than 2 days, 0 else
  */
int hasToLeaveHospital(inhabitants *inhabitant) {

    int isHospital = 0;
    sem_wait(citySem);
    boardCell currentCell = citySharedMemory->board[inhabitant->xPosition][inhabitant->yPosition];
    if(currentCell.type == hospital) {
        isHospital = 1;
    }
    sem_post(citySem);

    return (isHospital && inhabitant->job != doctor && inhabitant->timeElapsedSinceCuredInHospital>2);
}

/**
  * @brief Increase the contamination level of a cell based on its type
  * and the contamination level of the inhabitant who got in the cell
  * @param cell the cell which contamination will be increased
  * @param inhabitantContaminationLevel the contamination level of the entering inhabitant
  */
void increaseCellContaminationLevel(boardCell *cell, float inhabitantContaminationLevel) {

    switch (cell->type) {
        case wasteland:
        case house:
            cell->contaminationLevel += (float)(inhabitantContaminationLevel*0.01);
            break;
        case hospital:
            cell->contaminationLevel += (float)(inhabitantContaminationLevel*0.01*0.25);
            break;
        default:
            break;
    }
}

/**
  * @brief Return a boolean indicating if there is a firefighter in the cell at position x,y
  * @param x first coordinate of the cell
  * @param y second coordinate of the cell
  * @return 1 if a firefighter is in the cell, 0 else
  */
int isFireFighterInCell(int x, int y) {

    sem_wait(citySem);
    boardCell currentCell = citySharedMemory->board[x][y];
    for (int i = 0; i < currentCell.numberOfOccupants; i++) {

        int indexOfInhabitant = currentCell.residents[i];
        if (inhabitantsOfCity[indexOfInhabitant].job == fireFighter) {
            sem_post(citySem);
            return 1;
        }
    }
    sem_post(citySem);

    return 0;
}

/**
  * @brief Return a boolean indicating if the inhabitant is in a station without a firefighter
  * @param inhabitant the current inhabitant
  * @return 1 if there isn't a firefighter in the same cell that the inhabitant, 0 else
  */
int inStationWithoutFireFighter(inhabitants *inhabitant){

    if(inhabitant->job == fireFighter){
        return 0;
    }
    boardCell currentCell = citySharedMemory->board[inhabitant->xPosition][inhabitant->yPosition];
    int isStation = currentCell.type == station;
    return isStation && !isFireFighterInCell(inhabitant->xPosition, inhabitant->yPosition);
}


/**
  * @brief Choose randomly to move in one of the 8 neighbouring cells
  * @param deltaCoords an array of int indicating to which cell to move
  * @param generatorState seed for the random number generator
  */
void chooseCellAmongNeighbors(int deltaCoords[2], unsigned int generatorState) {

    int chooseNeighbor = (rand_r(&generatorState)%(8-1 +1)) + 1;
    switch (chooseNeighbor) {

        case 1:
            deltaCoords[0] = 0;
            deltaCoords[1] = 1;
            break;
        case 2:
            deltaCoords[0] = 1;
            deltaCoords[1] = 1;
            break;
        case 3:
            deltaCoords[0] = 1;
            deltaCoords[1] = 0;
            break;
        case 4:
            deltaCoords[0] = 1;
            deltaCoords[1] = -1;
            break;
        case 5:
            deltaCoords[0] = 0;
            deltaCoords[1] = -1;
            break;
        case 6:
            deltaCoords[0] = -1;
            deltaCoords[1] = -1;
            break;
        case 7:
            deltaCoords[0] = -1;
            deltaCoords[1] = 0;
            break;
        case 8:
            deltaCoords[0] = -1;
            deltaCoords[1] = 1;
            break;
        default:
            deltaCoords[0] = 0;
            deltaCoords[1] = 0;
            break;
    }
}

/**
  * @brief Return the position of the index of an habitant in an array full of index of inhabitants
  * @param indexOfInhabitant index of an inhabitant in the array inhabitants of the struct town
  * @param inhabitantsTab an array containing int corresponding to inhabitant
  * index from the inhabitants array of the struct town
  * @param sizeOfTab the size of inhabitantTab
  * @return the position of indexOfInhabitant in inhabitantsTab if it is in it, -1 else
  */
int findIndexOfInhabitant(int indexOfInhabitant, const int *inhabitantsTab, int sizeOfTab) {

    int i;
    for(i=0;i<sizeOfTab;i++) {
        if(indexOfInhabitant == inhabitantsTab[i]) {
            return i;
        }
    }

    return -1;
}

/**
  * @brief Add an inhabitant in a cell's array of residents
  * @param indexOfInhabitant the index of inhabitant to add
  * @param inhabitantsTab the resident array of the cell
  * @param sizeofTab size of inhabitantsTab
  */
void addInhabitantToCell(int indexOfInhabitant, int *inhabitantsTab, int *sizeofTab) {
    inhabitantsTab[*sizeofTab] = indexOfInhabitant;
    (*sizeofTab)++;
}

/**
  * @brief Remove an inhabitant from a cell's array of residents
  * @param indexToRemove the index of the inhabitant to remove
  * @param inhabitantsTab the resident array from the cell
  * @param sizeOfTab size of inhabitantsTab
  */
void removeInhabitants(int indexToRemove, int *inhabitantsTab, int *sizeOfTab) {

    int tmp;
    tmp = inhabitantsTab[(*sizeOfTab)-1];
    inhabitantsTab[indexToRemove] = tmp;
    (*sizeOfTab)--;
}

/**
  * @brief Return a boolean indicating if the inhabitant can move in the cell at position x,y
  * @param inhabitant current inhabitant
  * @param x first coordinate of the cell the inhabitant want to goes into
  * @param y second coordinate of the cell the inhabitant want to goes into
  * @return 1 if the inhabitant can go in the cell, 0 if the cell is already full or
  * if the inhabitant doesn't have the right to go into
  */
int cantMoveInCell(inhabitants *inhabitant,int x, int y) {

    sem_wait(citySem);
    boardCell currentCell = citySharedMemory->board[x][y];
    if(currentCell.numberOfOccupants == currentCell.capacity) {
        sem_post(citySem);

        return 0;
    }

    switch (currentCell.type) {
        case station:
            sem_post(citySem);
            return inhabitant->job == fireFighter || isFireFighterInCell(x,y);
        case hospital:
            sem_post(citySem);
            return canEnterInHospital(inhabitant);
        default:
            sem_post(citySem);
            return 1;
    }
}

/**
  * @brief The inhabitant is added in the cell at position newX newY, if they are a
  * firefighter they burn the dead people, if the cell is a station they get spray,
  * if the inhabitant is a doctor and the cell an hospital they get 10 doses
  * @param inhabitant current inhabitant
  * @param newX first coordinate of the cell the inhabitant goes into
  * @param newY seconde coordinate of the cell the inhabitant goes into
  * @param indexOfInhabitant index of the inhabitant in the inhabitant array of the city
  */
void goInCell(inhabitants *inhabitant, int newX, int newY, int indexOfInhabitant) {

    inhabitant->xPosition = newX;
    inhabitant->yPosition = newY;
    sem_wait(citySem);
    boardCell *currentCell = &citySharedMemory->board[inhabitant->xPosition][inhabitant->yPosition];
    addInhabitantToCell(indexOfInhabitant,currentCell->residents,&currentCell->numberOfOccupants);

    increaseCellContaminationLevel(currentCell, inhabitant->contaminationLevel);
    sem_post(citySem);

    if (inhabitant->job == doctor && currentCell->type == hospital) {
        inhabitant->numberOfDoses+=10;
    }
    if(inhabitant->job == fireFighter && currentCell->type == station){
        inhabitant->remainingSpray = 10.0f;
    }
}

/**
  * @brief Remove an inhabitant from a cell they are in
  * @param inhabitant current inhabitant
  * @param indexOfInhabitant index of the inhabitant in the inhabitant's array of the city
  */
void goOutOfCell(inhabitants *inhabitant, int indexOfInhabitant) {

    int indexInTab;
    inhabitant->timeOnSameCase = 0;

    sem_wait(citySem);

    boardCell *currentCell = &citySharedMemory->board[inhabitant->xPosition][inhabitant->yPosition];

    indexInTab = findIndexOfInhabitant(indexOfInhabitant,currentCell->residents,currentCell->numberOfOccupants);
    if(indexInTab == -1) {
        fprintf(stderr,"Erreur recherche habitant %d dans sa case, goOutOfCell\nCitizenManager va s'arrêter\n",indexOfInhabitant);
        exit(EXIT_FAILURE);
    }

    removeInhabitants(indexInTab,currentCell->residents,&currentCell->numberOfOccupants);

    sem_post(citySem);
}

/**
  * @brief Check if the inhabitant has to move, in which case the function decides to which neighbouring cell to go.
  * If the inhabitant can move in said cell, they will be removed from the current cell and then added to their
  * destination
  * @param inhabitant current inhabitant
  * @param generatorState a seed for the random number generator
  * @param index index of inhabitant in the inhabitant array of the city
  * @return 1 if the inhabitant has been moved, 0 else
  */
int move(inhabitants *inhabitant, unsigned int generatorState, int index) {

    double isMoving = (double)rand_r(&generatorState)/(double)RAND_MAX;
    if(isMoving<0.6 && !hasToLeaveHospital(inhabitant) && !inStationWithoutFireFighter(inhabitant)) {
        inhabitant->timeOnSameCase++;
        return 0;
    }

    int deltaCoords[2];
    chooseCellAmongNeighbors(deltaCoords,generatorState);
    int x = inhabitant->xPosition;
    int y = inhabitant->yPosition;

    int newX = x + deltaCoords[0];
    int newY = y + deltaCoords[1];

    if(newX < 0) {
        newX = 0;
    } else if(newX > 6) {
        newX = 6;
    }

    if(newY < 0) {
        newY = 0;
    } else if (newY > 6) {
        newY = 6;
    }

    if(!cantMoveInCell(inhabitant,newX,newY)) {
        return 0;
    }

    goOutOfCell(inhabitant, index);
    goInCell(inhabitant, newX, newY,index);

    return 1;
}

/**
  * @brief Calculate the new level of contamination of the inhabitant based on their job,
  * if they have moved and the cell they are in
  * @param cell the cell the inhabitant is in
  * @param individual the current inhabitant
  * @return the new level of contamination of the inhabitant
  */
float changeContaminationBasedOnCell(boardCell *cell, inhabitants *individual, int moved) {

    float contaminationCitizen;
    float contaminationCell;
    float contaminationModifier = 1.0f;
    if(inhabitantsOfCity->job == fireFighter){
        contaminationModifier = 0.1f;
    }

    contaminationCitizen = individual->contaminationLevel;
    sem_wait(citySem);
    contaminationCell = cell->contaminationLevel;
    sem_post(citySem);

    if(moved) {
        contaminationCitizen += contaminationModifier*contaminationCell*0.02f;
    } else {
        contaminationCitizen += contaminationModifier*contaminationCell*0.05f;
    }

    switch (cell->type) {
        case station:
            contaminationCitizen -= contaminationCitizen * 0.2f;
            break;
        case hospital:
            if(contaminationCitizen > contaminationCell) {
                contaminationCitizen -= contaminationCitizen * 0.1f;
            }
            individual->timeElapsedSinceOutOfHospital = 0;
            break;
        default:
            break;
    }

    return contaminationCitizen;
}

/**
  * @brief Verify if the contamination level of the inhabitant is in [0, 1]
  * @param contaminationCitizen the level of contamination of the inhabitant
  * @return 1 if the contamination level is superior to 1, 0 if it's inferior to
  * 0, else, the contamination level
  */
float capContamination(float contaminationCitizen) {
    if (contaminationCitizen > 1) {
        return 1;
    } else if (contaminationCitizen < 0) {
        return 0;
    }

    return contaminationCitizen;
}

/**
  * @brief If the inhabitant isn't dead, modify their contamination level
  * @param inhabitant the current inhabitant
  * @param moved a boolean indicating if the inhabitant has moved this turn
  */
void updateCitizenContamination(inhabitants *inhabitant, int moved) {

    float contaminationLevelOfCitizen;

    if (notDeadOrBurnt(inhabitant)) {
        sem_wait(citySem);
        boardCell *currentCell = &citySharedMemory->board[inhabitant->xPosition][inhabitant->yPosition];
        sem_post(citySem);

        contaminationLevelOfCitizen = changeContaminationBasedOnCell(currentCell, inhabitant, moved);
        contaminationLevelOfCitizen = capContamination(contaminationLevelOfCitizen);
        inhabitant->contaminationLevel = contaminationLevelOfCitizen;
    }
}

/**
  * @brief Update the status of the inhabitant if they get sick or died
  * @param individual the current inhabitant
  * @param generatorStateSick the seed for the random number generator used to
  * decide if the inhabitant is sick
  * @param generatorStateDead the seed for the random number generator used to
  * decide if the inhabitant is dead
  */
void updateCitizenStatus(inhabitants *individual, unsigned int generatorStateSick, unsigned int generatorStateDead) {

    double isSick = (double)rand_r(&generatorStateSick)/(double)RAND_MAX;
    double isDead = (double)rand_r(&generatorStateDead)/(double)RAND_MAX;

    if (willBeSick(individual, isSick)) {
        individual->condition = sick;
        sem_wait(statsSem);
        statsSharedMemory->numberOfContaminated++;
        individual->timeElapsedSinceCuredInHospital = 0;
        sem_post(statsSem);
    } else if (individual->condition == sick) {
        individual->timeElapsedSinceSick++;
    }

    if (individual->timeElapsedSinceSick >= 5) {
        if (isDead < 0.05*(individual->timeElapsedSinceSick-4)) {
            individual->condition = dead;
            sem_wait(citySem);
            citySharedMemory->nbInhabitantsAlive--;
            sem_post(citySem);
            sem_wait(statsSem);
            statsSharedMemory->numberOfContaminated--;
            sem_post(statsSem);
        }
    }

    boardCell currentCell = citySharedMemory->board[individual->xPosition][individual->yPosition];
    if(currentCell.type != hospital){
        individual->timeElapsedSinceOutOfHospital += 1;
    }else if(individual->condition == alive){
        individual->timeElapsedSinceCuredInHospital += 1;
    }
}

void *action(void *parameters) {

    int index = *(int *)parameters;
    int moved;
    unsigned int stateSick = time(NULL) ^ getpid() ^ pthread_self();
    unsigned int stateDead = time(NULL) ^ (getpid()/2) ^ pthread_self();
    unsigned int statePropagation = time(NULL) ^ getpid() ^ (pthread_self()/2);

    inhabitants *inhabitantX = &inhabitantsOfCity[index];

    if(isDeadOrBurnt(inhabitantX)) {
         pthread_barrier_wait(&barrier);
         infectedAction(inhabitantX, statePropagation);
        return NULL;
    }
    inhabitantX->hasHealed = 0;
    moved = move(inhabitantX,stateSick, index);
    updateCitizenContamination(inhabitantX, moved);
    updateCitizenStatus(inhabitantX, stateSick, stateDead);
    pthread_barrier_wait(&barrier);

    switch (inhabitantX->job) {
        case journalist:
            JournalistAction(inhabitantX);
            break;
        case citizen:
            infectedAction(inhabitantX, statePropagation);
            break;
        case doctor:
            inhabitantX->hasHealed = 0;
            doctorAction(inhabitantX);
            break;
        case fireFighter:
            firefighterAction(inhabitantX);
            break;
        default:
            break;
    }

    return NULL;
}

void createThreadForEachInhabitant() {

    int i;
    int indexes[POPULATION_NUMBER];
    pthread_t tid[POPULATION_NUMBER];

#ifdef CONSOLE_MODE
    fprintf(stderr,"Turn launched for citizens\n");
    printTownState(citySharedMemory->inhabitant, POPULATION_NUMBER);

#endif

    pthread_barrier_init(&barrier, NULL, POPULATION_NUMBER);
    for(i=0;i<POPULATION_NUMBER;i++) {
        indexes[i] = i;
        pthread_create(&tid[i], NULL, action, &indexes[i]);
    }

    for(i=0;i<POPULATION_NUMBER;i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

#ifdef CONSOLE_MODE
    fprintf(stderr,"End turn for citizens\n");
    fprintf(stderr,"----------------------------------\n");
#endif

}

/**
  * @brief Toggle on the boolean starting a turn
  */
void launchRound() {
    roundFlag = 1;
}

/**
  * @brief Toggle on the boolean ending citizenManager
  */
void endCitizenManager() {
    endGame = 1;
}

/**
  * @brief Close all the pipes, semaphores, mutex and shared memories of the program
  * @param toEpidemicSimFd the pipe between citizenManager and epidemicSim
  */
void prepareClosing(int toEpidemicSimFd) {
    close(toEpidemicSimFd);
    mq_close(pressChannel);
    sem_close(citySem);
    sem_close(statsSem);
    pthread_mutex_destroy(&channelMutex);
    pthread_mutex_destroy(&citizenMutex);
    munmap(citySharedMemory,sizeof(town));
    munmap(statsSharedMemory,sizeof(statistics));
    close(fdCity);
    close(fdStats);
}

int main() {

    int toEpidemicSimFd;
    int pid;
    char done = 'd';
    srand(time(NULL));

    getCity();
    getStats();
    getInhabitants();

    toEpidemicSimFd = createPipe(CITIZEN_CHANNEL);
    pid = getpid();
    write(toEpidemicSimFd,&pid,sizeof(int));

    pressChannel = openPressChannel(PRESS_CHANNEL);

    pthread_mutex_init(&channelMutex,NULL);
    pthread_mutex_init(&citizenMutex,NULL);
    initAlarmStruct(&launchRound, CITIZEN_HANDLING_COMMAND);
    initAlarmStruct(&endCitizenManager,END_OF_GAME);

    fprintf(stderr,"Start of CitizenManager\n");
    while(1) {
        pause();
        if(roundFlag) {
            createThreadForEachInhabitant();
            write(toEpidemicSimFd,&done,sizeof(char));
            roundFlag = 0;
        }
        if(endGame) {
            prepareClosing(toEpidemicSimFd);
            return 0;
        }
    }
}
