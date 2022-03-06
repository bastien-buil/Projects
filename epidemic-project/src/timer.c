/**
 * @file timer.c
 * @brief file of the programm timer who send signal to epidemicSim at each end
 * of loop
 * @author Jasmine Chaid-Akacem <jasmin.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#include "timer.h"

int durationOfTurn;
int epidemicPid;

volatile sig_atomic_t sendAlarm = 0;

/**
 * Check if th time passed as argument to the program is valid. i.e is in [1, 4]
 * @param argc the number of arguments
 * @param argv the arguments as a list of strings
 * @return 1 if the time is valid, 0 otherwise
 */
int isDurationValid(int argc, char* argv[]) {

    if (argc <= 1) {
        return 0;
    }
    int duration = atoi(argv[1]);
    return argc == 2 && duration > 0 && duration < 5;
}

/**
 * @brief End the timer program
 */
void endOfTimer() {
    exit(EXIT_SUCCESS);
}

void alarmEpidemicSim(__attribute__((unused)) int sig) {
    sendAlarm = 1;
}

int main(int argc, char* argv[]) {

    int fromEpidemicFd;
    int toEpidemicFd;
    int pid;

    if (isDurationValid(argc, argv)) {
        durationOfTurn = atoi(argv[1]);
    } else {
        printf("Timer: WARNING : Time not valid, initializing to 1s \n");
        durationOfTurn = 1;
    }


    signal(END_OF_TURN,  SIG_IGN);
    initAlarmStruct(&endOfTimer,END_OF_GAME);
    initAlarmStruct(&alarmEpidemicSim, START_OF_TURN);
    fromEpidemicFd = pipeConnector(PIDEPIDEMIC_CHANNEL);
    toEpidemicFd = createPipe(TIMER_CHAHNNEL);

    read(fromEpidemicFd, &epidemicPid, sizeof(int));
    close(fromEpidemicFd);

    pid = getpid();
    write(toEpidemicFd,&pid,sizeof(int));
    close(toEpidemicFd);

    alarm(durationOfTurn);

    while(1) {
        pause();
        if(sendAlarm) {
#ifdef CONSOLE_MODE
            fprintf(stderr,"+ %d s -> to epidemic of pid : %d\n", durationOfTurn, epidemicPid);
#endif
            kill(epidemicPid, START_OF_TURN);
            alarm(durationOfTurn);
            sendAlarm = 0;
        }
    }

    return 0;
}

