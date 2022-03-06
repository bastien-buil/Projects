/**
 * @file pressAgency.c
 * @brief File with the function used to receive and print the journalist
 * messages
 * @author Jasmine Chaid-Akacem <jasmin.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#include "pressAgency.h"
#include "utils.h"
#include <errno.h>
#include <limits.h>

mqd_t pressChannel;
FILE *logFile;
char *message;

void prepareClosingPressAgency() {
    free(message);
    mq_close(pressChannel);
    mq_unlink(PRESS_CHANNEL);
#if !defined(CONSOLE_MODE) && !defined(SILENCE_MODE)
    fclose(logFile);
    remove(PATH_TO_NEWS_LOG);
#endif
}

void resetMessage(char *_message,int size) {

    int i;
    for(i=0; i<size; i++) {
        _message[i] = '\0';
    }
}

int displayJournalistContamination(int journalistContamination) {

    if(journalistContamination>=80) {
#if defined(CONSOLE_MODE) || defined(SILENCE_MODE)
        printf("Taux de contamination journaliste: %d %% \n",journalistContamination);
#else
        fprintf(logFile, "Taux de contamination journaliste: %d %% \n",journalistContamination);
        fflush(logFile);
#endif
        return journalistContamination;
    }

    return -1;
}

double displayCityContamination(float cityContamination) {

    double displayedNumber = cityContamination - cityContamination*0.10;
#if defined(CONSOLE_MODE) || defined(SILENCE_MODE)
    printf("Taux de contamination général: %f %% \n",displayedNumber);
#else
    fprintf(logFile, "Taux de contamination général: %f %% \n",displayedNumber);
    fflush(logFile);
#endif
    return displayedNumber;
}

int displayDeathToll(int deathToll) {

    int displayedNumber = (int)(deathToll - deathToll*0.35);
#if defined(CONSOLE_MODE) || defined(SILENCE_MODE)
    printf("Nombre de morts: %d\n", displayedNumber);
#else
    fprintf(logFile, "Nombre de morts: %d\n", displayedNumber);
    fflush(logFile);
#endif

    return displayedNumber;
}

int displayContaminationToll(int contaminationToll) {

    int displayedNumber = (int)(contaminationToll - contaminationToll*0.10);
#if defined(CONSOLE_MODE) || defined(SILENCE_MODE)
    printf("Nombre de citoyens contaminés: %d\n",displayedNumber);
#else
    fprintf(logFile, "Nombre de citoyens contaminés: %d\n",displayedNumber);
    fflush(logFile);
#endif
    return displayedNumber;
}

int conversionFromCharToInt(const char *number) {

    char *eptr;
    long result;

    result = strtol(number,&eptr,10);
    if (result == 0) {
        if (errno == EINVAL) {
            fprintf(stderr,"Conversion error occurred: %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    if (result == LONG_MIN || result == LONG_MAX) {
        if (errno == ERANGE) {
            fprintf(stderr, "TValue provided out of range\n");
            exit(EXIT_FAILURE);
        }
    }

    return (int)result;
}

float conversionFromCharToFloat(const char *number) {

    char *eptr;
    double result;

    result = strtod(number,&eptr);
    if (result == 0) {
        if (errno == EINVAL) {
            fprintf(stderr,"Conversion error occurred: %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    return (float)result;
}

void readAMessage(mqd_t _pressChannel) {
    unsigned int priority;
    struct mq_attr attr;
    int size;

    mq_getattr(_pressChannel,&attr);
    size = mq_receive(_pressChannel,message,attr.mq_msgsize,&priority);
    switch (priority) {
        case JOURNALIST_CONTAMINATION:
            displayJournalistContamination(conversionFromCharToInt(message));
            break;
        case CITY_CONTAMINATION:
            displayCityContamination(conversionFromCharToFloat(message));
            break;
        case DEATH_TOLL:
            displayDeathToll(conversionFromCharToInt(message));
            break;
        case CONTAMINATION_TOLL:
            displayContaminationToll(conversionFromCharToInt(message));
            break;
        default:
            fprintf(stderr,"Priority unknown, skip \n");
    }

    resetMessage(message,size);
}

void initMessage(mqd_t _pressChannel){
    struct mq_attr attr;

    mq_getattr(_pressChannel,&attr);
    message = calloc(attr.mq_msgsize, 1);
    if(message == NULL) {
        fprintf(stderr,"Erreur lors de l'allocation du buffer message dans PressAgency\nPressAgency va stopper\n");
        exit(EXIT_FAILURE);
    }
}

void openLog(){
    logFile = fopen(PATH_TO_NEWS_LOG,"a");
    if(logFile==NULL){
        perror("log file couldn't be opened\n");
        exit(EXIT_FAILURE);
    }
}

void endProgram() {
    prepareClosingPressAgency();
    exit(EXIT_SUCCESS);
}

int main() {

    int toEpidemicSimFd;
    int pid;
    pid = getpid();
    remove(PATH_TO_NEWS_LOG);
#if !defined(CONSOLE_MODE) && !defined(SILENCE_MODE)
    openLog();
#endif
    toEpidemicSimFd = createPipe(PRESSAGENCY_CHANNEL);
    write(toEpidemicSimFd,&pid,sizeof(int));
    close(toEpidemicSimFd);

    pressChannel = createPressChannel(PRESS_CHANNEL);

    initAlarmStruct(endProgram,END_OF_GAME);
    initMessage(pressChannel);
    while(1) {
        readAMessage(pressChannel);
    }
    return 0;
}
