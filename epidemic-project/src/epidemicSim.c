/**
 * @file epidemicSim.c
 * @brief File with the function used to run the simulation
 * @author Jasmine Chaid-Akacem <jasmin.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#include <sys/wait.h>
#include <time.h>
#include "citizenManager.h"
#include "epidemicSim.h"

volatile sig_atomic_t roundFlag = 0;

int turnNumber = 0;
int pidCitizenManager;
int pidPressAgency;
int pidTimer;

sem_t *townSem;
sem_t *statsSem;

pcg32_random_t rng1;

void startOfTurnEvent(__attribute__((unused)) int sig) {
    if(!roundFlag) {
        turnNumber++;
        roundFlag = 1;
    }
}

/**
 * @brief send a signal to citizenManager for it to begin a new turn
 */
void sendSignalToCitizenManager() {
    kill(pidCitizenManager,CITIZEN_HANDLING_COMMAND);

}

/**
 * @brief send a signal to citizenManager for it to begin a new turn
 */
void sendEndOfRoundToTimer() {
    kill(pidTimer, END_OF_TURN);

}

/**
 * @brief Create a semaphore on the town
 */
void createTownSemaphore() {

    townSem = sem_open(SEMTOWN,O_CREAT, 0644, 1);

    if(townSem == SEM_FAILED) {
        fprintf(stderr,"Erreur création semaphore %s\nepidemicSim va quitter\n", SEMTOWN);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Create a semaphore on the statistics
 */
void createStatsSemaphore() {

    statsSem = sem_open(SEMSTATS,O_CREAT,0644,1);
    if(statsSem == SEM_FAILED) {
        fprintf(stderr,"Erreur création semaphore %s\nepidemicSim va quitter\n", SEMSTATS);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Create the shared memory of the town
 * @return a file descriptor to the shared memory
 */
int createTownSharedMemory(){
    int fdShm;
    fdShm = shm_open(MEMTOWN,O_CREAT|O_RDWR, 0777);

    if (fdShm == -1){
        perror("error opening mem");
        exit(EXIT_FAILURE);
    }

    if(ftruncate(fdShm,sizeof(town)) == -1){
        perror("error ftruncate");
        exit(EXIT_FAILURE);
    }

    createTownSemaphore();
    return fdShm;
}

/**
 * @brief Create the shared memory of the statistics
 * @return a file descriptor to the shared memory
 */
int createStatsSharedMemory(){
    int fdStats;

    fdStats = shm_open(MEMSTATS,O_CREAT|O_RDWR,0644);

    if(fdStats == -1) {
        perror("error opening mem");
        exit(EXIT_FAILURE);
    }

    if(ftruncate(fdStats,sizeof(statistics)) == -1) {
        perror("error ftruncate");
        exit(EXIT_FAILURE);
    }
    createStatsSemaphore();
    return fdStats;
}

/**
 * @brief Get the city form the shared memory
 * @param fdShmTown the file descriptor of the shared memory
 * @return a pointer to the new city
 */
town* getCityFromSharedMemory(int fdShmTown) {

    town *city;
    city = mmap(NULL, sizeof(town), PROT_READ|PROT_WRITE, MAP_SHARED, fdShmTown, 0);

    if(city == NULL) {
        fprintf(stderr, "Erreur projection mémoire partagée");
    }
    return city;
}

/**
 * @brief Get the statistics form the shared memory
 * @param fdShmStats the file descriptor of the shared memory
 * @return a pointer to the new statistics
 */
statistics *openStatistics(int fdShmStats) {

    statistics *stats;
    stats = mmap(NULL,sizeof(statistics),PROT_READ|PROT_WRITE,MAP_SHARED,fdShmStats,0);

    if(stats == NULL) {
        fprintf(stderr,"Erreur projection mémoire partagée");
    }

    return stats;
}

/**
 * @brief Initialize the statistics
 * @param stats
 */
void initStatistics(statistics *stats) {

    stats->averageContaminationLevelOfTown = 0.0f;
    stats->numberOfContaminated = 0;
    stats->numberOfDeath = 0;
    stats->numberOfBurnt = 0;
}

/**
 * @brief Show a minimalistic ASCII version of the simulation
 * @param _town the town to show
 */
void printTownInhabitantMatrix(town *_town) {
    int i;
    int j;
    for(i=0;i < WIDTH;i++) {
        for (j = 0; j < HEIGHT; j++) {
            printf("%d /%4.f %% | ", _town->board[i][j].numberOfOccupants, _town->board[i][j].contaminationLevel*100);
        }

        printf("\n");
    }
}

/**
 * @brief calculate the median level of contamination
 * @param town
 * @return
 */
float calculateAverageContaminationLevel(town *town) {

    float acc = 0.0f;

    for(int i=0;i<WIDTH;i++) {
        for(int j=0;j<HEIGHT;j++) {
            acc+=town->board[i][j].contaminationLevel;
        }
    }

    return (acc/(WIDTH*HEIGHT));

}

/**
 * @brief update the statistics of a simulation each turn
 * @param simulation the simulation to take the statistics from
 */
void updateStatistics(simulationInformation *simulation) {

    statistics *stats = simulation->townStats;
    stats->numberOfDeath = POPULATION_NUMBER - simulation->_town->nbInhabitantsAlive;
    stats->averageContaminationLevelOfTown = calculateAverageContaminationLevel(simulation->_town);

}

/**
 * @brief Initialize the inhabitant passed as argument in position x, y
 * @param inhabitant the inhabitant to initialize
 * @param _jobs the job of said inhabitant
 * @param x their x position on the board
 * @param y their y position on the board
 */
void initInhabitant(inhabitants *inhabitant, jobs _jobs, int x, int y){

	inhabitant->job = _jobs;
    inhabitant->condition = alive;
	inhabitant->timeElapsedSinceSick = 0;
	inhabitant->timeElapsedSinceCuredInHospital = 0;
	inhabitant->timeElapsedSinceOutOfHospital = 3;
	inhabitant->timeOnSameCase = 0;
    inhabitant->hasHealed = 0;

	if(_jobs == doctor){
		inhabitant->numberOfDoses = 5;
	}
	if(_jobs == fireFighter){
		inhabitant->remainingSpray = 5.0f;
	}
	inhabitant->xPosition = x;
	inhabitant->yPosition = y;
	inhabitant->contaminationLevel = 0;
}

/**
 * @brief Initialize the differents places on the board
 * @param _town the town in which the buildings will be placed
 */
void initPlaces(town *_town){

	int i;
	int j;
    int x;
    int y;
    int nbHouse = NB_HOUSE;
    int coords[2];

	for(i=0;i < WIDTH;i++){
		for(j=0;j < HEIGHT;j++){
			boardCell cell;
			cell.xPosition = i;
			cell.yPosition = j;
            cell.numberOfOccupants = 0;
            cell.capacity = 16;
            cell.type = wasteland;
            cell.contaminationLevel = 0;
			_town->board[i][j] = cell;
		}
	}

	_town->board[3][3].type = hospital;
	_town->board[3][3].capacity = 12;
	
	_town->board[0][6].type = station;
	_town->board[0][6].capacity = 8;
	
	_town->board[6][0].type = station;
	_town->board[6][0].capacity = 8;

	while(nbHouse > 0){
        getRandomCoords(coords, &rng1, 7);
        x = coords[0];
        y = coords[1];
		if(_town->board[x][y].type == wasteland){
			_town->board[x][y].type = house;
			_town->board[x][y].capacity = 6;
			nbHouse--;
		}
	}
}

/**
 * @brief Check if the current cell is an already contaminated wasteland
 * @param _town the town in which the cell is
 * @param x its w position on the board
 * @param y its y position on the board
 * @return 1 if it is already contaminated
 */
int isWastelandNotContaminationInit(town *_town, int x, int y) {
    return _town->board[x][y].type == wasteland && _town->board[x][y].contaminationLevel == 0;
}

/**
 * @brief Initialize a random contamination level [in between 20 - 40]% on a certain number of wastelands
 * @param _town the town in which the wasteland is
 */
void initWastelandContamination(town *_town) {
    int nbBaseWastelandContaminated;
    int x;
    int y;
    int coords[2];

    nbBaseWastelandContaminated = ((WIDTH*HEIGHT) - NB_HOUSE - NB_HOSPITAL - NB_STATION)/10;
    while(nbBaseWastelandContaminated > 0){

        getRandomCoords(coords, &rng1, 7);
        x = coords[0];
        y = coords[1];
        if(isWastelandNotContaminationInit(_town, x, y)){
            _town->board[x][y].contaminationLevel = (float)(0.1*(getRandom32bitNumber(&rng1)%3+2));
            nbBaseWastelandContaminated--;
        }
    }
}

/**
 * @brief Check if a journalist can be placed on a cell (as there are no conditions it is a trivial function, but nonetheless
 * important for the initInhabList function)
 * @param _town the town in which the journalist is
 * @param x their x position
 * @param y their y position
 * @return 1 if they can be placed
 */
int canJournalistBePlaced(__attribute__((unused)) town *_town, __attribute__((unused)) int x, __attribute__((unused)) int y) {
    return 1;
}

/**
 * @brief Check if a doctor can be placed on a cell
 * @param _town the town in which the doctor resides
 * @param x their x position
 * @param y their y position
 * @return 1 if they can be placed
 */
int canDoctorBePlaced(town *_town, int x, int y) {
    int isCellStation = _town->board[x][y].type == station;
    int isFull = _town->board[x][y].capacity == _town->board[x][y].numberOfOccupants;

    return !(isCellStation || isFull);
}

/**
 * @brief Check if a firefighter can be placed on a cell
 * @param _town the town in which the firefighter resides
 * @param x their x position
 * @param y their y position
 * @return 1 if they can be placed
 */
int canFirefighterBePlaced(town* _town, int x, int y) {
    int isCellHospital = _town->board[x][y].type == hospital;
    int isFull = _town->board[x][y].capacity == _town->board[x][y].numberOfOccupants;

    return !(isCellHospital || isFull);
}

/**
 * @brief Check if a citizen can be placed on a cell
 * @param _town the town in which the citizen resides
 * @param x their x position
 * @param y their y position
 * @return 1 if they can be placed
 */
int canCitizenBePlaced(town *_town, int x, int y) {
    int isCellHospital = _town->board[x][y].type == hospital;
    int isCellStation = _town->board[x][y].type == station;
    int isFull = _town->board[x][y].capacity == _town->board[x][y].numberOfOccupants;

    return !(isCellHospital || isCellStation || isFull);
}

/**
 * @brief Add an individual in the town at position (x, y)
 * @param _town the town to add the individual in
 * @param individual the individual to add
 * @param residentNumber its index in the array of inhabitant
 * @param x their x position
 * @param y their y position
 */
void addInTown(town *_town, inhabitants individual, int residentNumber, int x, int y) {
    int indexInCell;

    _town->inhabitant[residentNumber] = individual;
    indexInCell = _town->board[x][y].numberOfOccupants;
    _town->board[x][y].residents[indexInCell] = residentNumber;
    _town->board[x][y].numberOfOccupants++;
}

/**
 * @brief Initialize and add the default people in the hospital and stations
 * @param _town the town in which the people will be added
 */
void initAndAddBaseDoctorAndFighter(town *_town) {
    inhabitants doctor1;
    inhabitants fireFighter1;
    inhabitants fireFighter2;

    initInhabitant(&doctor1, doctor, 4, 4);
    initInhabitant(&fireFighter1, fireFighter, 0, 6);
    initInhabitant(&fireFighter2, fireFighter, 6, 0);

    addInTown(_town, doctor1, 0, 4, 4);
    addInTown(_town, fireFighter1, 1, 0, 6);
    addInTown(_town, fireFighter2, 2, 6, 0);
}

/**
 * @brief Add inhabitants to the town. The way it works is that each group of people is represented as an interval
 * e.g firefighter = [0, 6]. Thus, for each index in [0, TOTAL] the function will check if said index is in the intervals
 * of each type of people and add them if this is the case.
 * @param _town the town to add the citizen in
 */
void initInhabitantListe(town *_town){
    int index;
    int typeIndex;
    int residentNumber;
    int coords[2];

    int (*canBePlaced[NUMBER_OF_TYPE])(town*, int, int) = {&canJournalistBePlaced, &canDoctorBePlaced, &canFirefighterBePlaced, &canCitizenBePlaced};
    int remainingCitizenType[NUMBER_OF_TYPE] = {REMAINING_JOURNALIST, REMAINING_DOCTOR, REMAINING_FIRE, REMAINING_CITIZEN};
    int typeOfCitizen[NUMBER_OF_TYPE] = {journalist, doctor, fireFighter, citizen};

    residentNumber = 3;
    getRandomCoords(coords, &rng1, 7);

    _town->nbInhabitantsAlive = POPULATION_NUMBER;
    initAndAddBaseDoctorAndFighter(_town);

    for (index=0; index < REMAINING_CITIZEN; index++) {

        inhabitants individual;
        for (typeIndex=0; typeIndex < NUMBER_OF_TYPE; typeIndex++) {

            if (isInBetween(index, 0, remainingCitizenType[typeIndex])) {
                while(!canBePlaced[typeIndex](_town, coords[0], coords[1])){
                    getRandomCoords(coords, &rng1, 7);
                }

                initInhabitant(&individual, typeOfCitizen[typeIndex], coords[0], coords[1]);
                addInTown(_town, individual, residentNumber, coords[0], coords[1]);
                residentNumber++;
                getRandomCoords(coords, &rng1, 7);
            }
        }
    }
}

/**
 * @brief Initialize the whole town based on previous functions
 * @param _town the town to initialize
 */
void initTown(town *_town){
	initPlaces(_town);
    initWastelandContamination(_town);
	initInhabitantListe(_town);
}


/**
 * @brief calculate the propagation of contamination on wasteland
 * @param city
 */
void propagateWasteLand(town *city){
    int caseToTest[WIDTH*HEIGHT];
    int maxCase = WIDTH*HEIGHT;
    for(int i=0; i < WIDTH*HEIGHT; i++) {
        caseToTest[i] = i;
    }
    while(maxCase > 0){
        int randomI = (int)pcg32_boundedrand_r(&rng1, maxCase);
        int x = caseToTest[randomI] % WIDTH;
        int y = caseToTest[randomI] / WIDTH;
        boardCell* currentCell = &city->board[x][y];
        if(currentCell->type != wasteland){
            maxCase--;
            caseToTest[randomI] = caseToTest[maxCase - 1];
            continue;
        }
        for(int i = 0; i < 9; i++){
            int dx = i % 3 - 1;
            int dy = i / 3 - 1;
            int nx = x + dx;
            int ny = y + dy;
            if((dx == 0 && dx == dy) || nx < 0 || nx >= WIDTH ||  ny < 0 || ny >= HEIGHT) {
                continue;
            }
            boardCell* newCell = &city->board[nx][ny];
            if(newCell->type == wasteland && (currentCell->contaminationLevel > newCell->contaminationLevel)){
                if(pcg32_boundedrand_r(&rng1, 100) < 15){
                    float percentOfSpreading = (float)(1 + pcg32_boundedrand_r(&rng1, 20))/100;
                    newCell->contaminationLevel += (currentCell->contaminationLevel - newCell->contaminationLevel) * percentOfSpreading;
                }
            }

        }
        maxCase--;
        caseToTest[randomI] = caseToTest[maxCase - 1];

    }

}

void updateEvolutionTxt(FILE *file, simulationInformation *sim){

	unsigned int currTurn;
	unsigned int nbAlive;
	unsigned int nbIll;
	unsigned int nbDead;
	unsigned int nbBurnt;

	currTurn = sim->currentTurn;
	nbAlive = sim->_town->nbInhabitantsAlive;
	nbIll = sim->townStats->numberOfContaminated;
	nbDead = sim->townStats->numberOfDeath;
	nbBurnt = sim->townStats->numberOfBurnt;
	fprintf(file,"%d %d %d %d %d\n", currTurn, nbAlive, nbIll, nbDead, nbBurnt);
}

/**
 * @brief execute gnuplot in order to create a graph of the statistics
 */
void executeGnuplot(){
    execlp("gnuplot", "gnuplot", "-persist", "commandes.gp", NULL);
}

/**
 * @brief close all of the files used during the simulation
 * @param city
 * @param stats
 * @param fdShmTown
 * @param fdToCitizenManager
 */
void prepareClosing(town *city, statistics *stats, int fdShmTown, int fdToCitizenManager) {

    munmap(city, sizeof(town));
    munmap(stats, sizeof(statistics));
    sem_close(townSem);
    sem_close(statsSem);
    close(fdShmTown);
    close(fdToCitizenManager);
    removeFile(PATH_TO_EVOLUTION);
}

int main(){

    // initalize the random number generator. For more information see utils.h
    pcg32_srandom_r(&rng1, time(NULL), (intptr_t)&rng1);

	int fdShmTown;
    int fdShmStats;
    int pid;
    int toTimerFd;
    int fromCitizenManagerFd;
    int fromPressAgencyFd;
    int fromTimerFd;
    int arrayOfPid[3];

    simulationInformation simulation;
    town *city;
    statistics *stats;
    FILE *file;

    unlinkAll();
    removeFile(PATH_TO_EVOLUTION);

    fdShmTown = createTownSharedMemory();
    city = getCityFromSharedMemory(fdShmTown);
    initTown(city);
	simulation._town = city;
    file = openFile(PATH_TO_EVOLUTION, O_FAPPEND);

    fdShmStats = createStatsSharedMemory();
    stats = openStatistics(fdShmStats);
    initStatistics(stats);
    simulation.townStats = stats;
	simulation.currentTurn = 0;


    toTimerFd = createPipe(PIDEPIDEMIC_CHANNEL);

    fromPressAgencyFd = pipeConnector(PRESSAGENCY_CHANNEL);
    read(fromPressAgencyFd,&pidPressAgency,sizeof(int));
    close(fromPressAgencyFd);
    arrayOfPid[0] = pidPressAgency;

    fromCitizenManagerFd = pipeConnector(CITIZEN_CHANNEL);
    read(fromCitizenManagerFd,&pidCitizenManager,sizeof(int));
    pid = getpid();
    write(toTimerFd, &pid, sizeof(int));
    close(toTimerFd);
    arrayOfPid[1] = pidCitizenManager;

    sleep(1);
    fromTimerFd = pipeConnector(TIMER_CHAHNNEL);
    read(fromTimerFd,&pidTimer,sizeof(int));
    close(fromTimerFd);
    arrayOfPid[2] = pidTimer;

    initAlarmStruct(&startOfTurnEvent, START_OF_TURN);

    fprintf(stderr,"Start of epidemicSim\n");

    #ifdef CONSOLE_MODE

    fprintf(stderr,"Initial town:\n");

    printTownInhabitantMatrix(city);

    #endif

    do {
        pause();
        if(roundFlag) {
            char done;
            sendSignalToCitizenManager();

            while(read(fromCitizenManagerFd,&done,sizeof(char)) == -1);
            simulation.currentTurn = turnNumber;
            updateStatistics(&simulation);
            propagateWasteLand(city);
#ifdef CONSOLE_MODE
            fprintf(stderr,"Turn %d\n",turnNumber);
            printTownInhabitantMatrix(city);
#endif
            roundFlag = 0;
            sendEndOfRoundToTimer();
	        updateEvolutionTxt(file, &simulation);
        }

    } while (turnNumber < TURN_MAX);
    fclose(file);

    pid_t pid2;
    pid2 = fork();
    if(pid2 < 0){
	    perror("error using fork");
	    exit(EXIT_FAILURE);
    }

    if (pid2 > 0){
	    wait(NULL);
    } else {
	    executeGnuplot();
    }

    endPrograms(arrayOfPid, 3);
    prepareClosing(city, stats, fdShmTown, fromCitizenManagerFd);
    unlinkAll();
    return 0;
}

