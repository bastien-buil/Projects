#include "utils.h"
#include "mainWindow.h"
#include "windowConnector.h"

windowinfo* windowInfoForSignal = NULL;

void connectPipes(windowinfo* windowInfo){
    int pid;

    int toEpidemicFd;
    mkfifo(TIMER_CHAHNNEL, 0777);

    int fromEpidemicFd = pipeConnector(PIDEPIDEMIC_CHANNEL);
    read(fromEpidemicFd, &windowInfo->epidemicPid, sizeof(int));
    close(fromEpidemicFd);

    toEpidemicFd = open(TIMER_CHAHNNEL, O_CREAT|O_WRONLY);
    if (toEpidemicFd == -1) {
        perror("could not create the pipe");
        exit(EXIT_FAILURE);
    }

    pid = getpid();
    write(toEpidemicFd,&pid,sizeof(int));
    close(toEpidemicFd);
}


/**
 * @brief Update stats and day and main map
 * @param windowInfo
 */
void updateInterface(windowinfo* windowInfo){
    displayStatsAndDay(windowInfo);
    gtk_widget_queue_draw(windowInfo->widgets.mapsDraw);
}

void sendNewTurn(windowinfo* windowInfo){
    if(!windowInfo->roundOnGoing){
        windowInfo->roundOnGoing = TRUE;
        kill(windowInfo->epidemicPid, START_OF_TURN);
    }
}


gboolean endOfTurnCB(gpointer data){
    windowinfo* windowInfo = data;
    windowInfo->turn_number++;
    updateInterface(windowInfo);
    readLog(windowInfo);
    windowInfo->roundOnGoing = FALSE;
    return FALSE;
}

void endOfTurn(__attribute__((unused))int sig){
    g_idle_add(G_SOURCE_FUNC(endOfTurnCB), windowInfoForSignal);
}

gboolean endOfGameCB(gpointer data){
    windowinfo* windowInfo = data;
    updateInterface(windowInfo);
    windowInfo->roundOnGoing = TRUE;
    gtk_label_set_text(GTK_LABEL(windowInfo->widgets.playNextTurnLabel), "Simulation terminée");
    gtk_widget_show_all(windowInfo->widgets.playNextTurnLabel);
    readLog(windowInfo);
    updateImage(windowInfo);
    return FALSE;
}

void endOfGame(__attribute__((unused))int sig){
    g_idle_add(G_SOURCE_FUNC(endOfGameCB), windowInfoForSignal);
}

void setupSignal(windowinfo* windowInfo){
    if(windowInfoForSignal != NULL){
        printf("Global variable has already been defined, then it's mean that setupSignal has been called 2 times. And this is prohibited.");
        exit(EXIT_FAILURE);
    }
    windowInfoForSignal = windowInfo;
    initAlarmStruct(&endOfTurn,END_OF_TURN);
    initAlarmStruct(&endOfGame,END_OF_GAME);
}

void loadSharedMemory(windowinfo* windowInfo){
    windowInfo->fdCity = shm_open(MEMTOWN,O_RDWR,S_IRUSR | S_IWUSR);
    if(windowInfo->fdCity == -1) {
        fprintf(stderr,"Erreur ouverture mémoire partagée %s\nwindow va quitter\n",MEMTOWN);
        exit(EXIT_FAILURE);
    }

    windowInfo->_town = mmap(NULL,sizeof(town),PROT_READ,MAP_SHARED,windowInfo->fdCity,0);
    if(windowInfo->_town == NULL) {
        fprintf(stderr, "Erreur projection mémoire partagée %s\nwindow va quitter\n", MEMTOWN);
        exit(EXIT_FAILURE);
    }


    windowInfo->fdStats = shm_open(MEMSTATS,O_RDWR,S_IRUSR | S_IWUSR);
    if(windowInfo->fdStats == -1) {
        fprintf(stderr,"Erreur ouverture mémoire partagée %s\nwindow va quitter\n",MEMTOWN);
        exit(EXIT_FAILURE);
    }

    windowInfo->townStats = mmap(NULL,sizeof(statistics),PROT_READ,MAP_SHARED,windowInfo->fdStats,0);
    if(windowInfo->townStats == NULL) {
        fprintf(stderr, "Erreur projection mémoire partagée %s\nwindow va quitter\n", MEMTOWN);
        exit(EXIT_FAILURE);
    }
}

void closeSharedMemoryAndFiles(windowinfo* windowInfo){

    munmap(windowInfo->_town,sizeof(town));
    close(windowInfo->fdCity);

    munmap(windowInfo->townStats,sizeof(statistics));
    close(windowInfo->fdStats);

    fclose(windowInfo->logFile);
}


void readLog(windowinfo* windowInfo){
    char* text = NULL;
    size_t sizeRead;
    while(getline(&text, &sizeRead, windowInfo->logFile) != -1){
        addNews(windowInfo, text);
    }
    free(text);
    clearerr(windowInfo->logFile);
}

void openNewsLog(windowinfo* windowInfo){
    windowInfo->logFile = fopen(PATH_TO_NEWS_LOG, "r");
    if(windowInfo->logFile == NULL){
        printf("Can't open pressAgency log file");
        exit(EXIT_FAILURE);
    };
}
