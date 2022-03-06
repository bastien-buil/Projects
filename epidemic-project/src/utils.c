/**
 * @file utils.c
 * @brief File with various usefull function used in various programms
 * @author Jasmine Chaid-Akacem <jasmin.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#include "utils.h"

void initAlarmStruct(void (*func)(int), int sig) {
    struct sigaction action;

    memset(&action, '\0', sizeof(action));
    action.sa_handler = func;
    sigaction(sig, &action, NULL);
}

void sendSignalToProgram(int pidOfProgram, int sig) {
    while (kill(pidOfProgram, sig) == -1) {
        usleep(20);
    }
}

void endPrograms(int *arrayOfPid, int size) {

    int i;
    for (i=0; i < size; i++) {
        sendSignalToProgram(arrayOfPid[i], END_OF_GAME);
    }
}

FILE *openFile(char* path, char* mode) {
    FILE *file;

    file = fopen(path,mode);
    if(file==NULL){
        perror("file couldn't be opened\n");
        exit(EXIT_FAILURE);
    }
    return file;
}

int removeFile(char* name) {

    remove(name);
    return 0;
}

int createPipe(char* channel) {
    int fdPipe;
    mkfifo(channel, 0777);
    fdPipe = open(channel, O_CREAT|O_WRONLY);

    if (fdPipe == -1) {
        perror("could not create the pipe");
        exit(EXIT_FAILURE);
    }

    return fdPipe;
}

int pipeConnector(char* channel) {
    int fdPipe;
    fdPipe = open(channel, O_RDONLY);

    if (fdPipe == -1) {
        perror("Could not connect to the pipe");
        exit(EXIT_FAILURE);
    }

    return fdPipe;
}

mqd_t createPressChannel(char *name) {

    mqd_t channel;
    channel = mq_open(name,O_RDONLY|O_CREAT,0644,NULL);

    if(channel == -1) {
        perror("Could not create pressChannel");
        exit(EXIT_FAILURE);
    }

    return channel;
}

mqd_t openPressChannel(char *name) {

    mqd_t channel;
    channel = mq_open(name,O_WRONLY);

    if(channel == -1) {
        perror("Could not open pressChannel");
        exit(EXIT_FAILURE);
    }

    return channel;

}

int isInBetween(int x, int a, int b) {
    return x >= a && x <= b;
}

void unlinkAll() {

    shm_unlink(MEMTOWN);
    shm_unlink(MEMSTATS);
    unlink(PIDEPIDEMIC_CHANNEL);
    unlink(CITIZEN_CHANNEL);
    unlink(PRESSAGENCY_CHANNEL);
    unlink(TIMER_CHAHNNEL);
    sem_unlink(SEMTOWN);
    sem_unlink(SEMSTATS);
}

uint32_t getRandom32bitNumber(pcg32_random_t *rng_ptr) {
    return pcg32_random_r(rng_ptr);

}

void getRandomCoords(int *coords, pcg32_random_t *rng_ptr, int bound) {
    coords[0] = pcg32_boundedrand_r(rng_ptr, bound);
    coords[1] = pcg32_boundedrand_r(rng_ptr, bound);
}
