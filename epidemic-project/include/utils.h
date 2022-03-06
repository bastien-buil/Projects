/**
 * @file utils.h
 * @brief This file contains general functions that can be used "anywhere" in the programs
 * @author Jasmine Chaid-Akacem <jasmine.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#ifndef PROJET_EPIDEMIE_UTILS_H
#define PROJET_EPIDEMIE_UTILS_H
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "pcg_basic.h"
#include "define.h"

/**
 * @brief Initialize the alarm structure for the signal sig that will use the function func on recieving said signal
 * @param func the function triggered at the signal reception
 * @param sig the signal to wait for
 */
void initAlarmStruct(void (*func)(int), int sig);


/**
 * @brief send a signal to a program. Tries until it manages to do so (every 20us)
 * @param pidOfProgram the pid of the program to send the signal to
 * @param sig the signal to send
 */
void sendSignalToProgram(int pidOfProgram, int sig);

/**
 * @brief end a group of programs
 * @param arrayOfPid the group of program to end
 * @param size the size of the array
 */
void endPrograms(int *arrayOfPid, int size);

/**
 * @brief open a file (check whether it can be opened or not)
 * @param path the path to the file
 * @param mode the mode in which the file will be opened
 * @return a FILE pointer
 */
FILE *openFile(char* path, char* mode);

/**
 * @brief Tries to remove a file
 * @param name the name of the file to remove
 * @return -1 if the file could not be removed, 0 otherwise
 */
int removeFile(char* name);

/**
 * @brief Create a pipe connecting two programs
 * @param channel the name of the channel
 * @return a descriptor to the channel that was created
 */
int createPipe(char* channel);

/**
 * @brief Allow for the connection to a pipe
 * @param channel the name of the pipe to connect to
 * @return a descriptor to the channel the program connected to
 */
int pipeConnector(char* channel);

/**
 * @brief Create a queue for two programs to exchange information
 * @param name the name of the queue
 * @return said queue
 */
mqd_t createPressChannel(char *name);

/**
 * Allow for the connection to a queue
 * @param name the name of the queue to connect to
 * @return said queue
 */
mqd_t openPressChannel(char *name);

/**
 * Check if a number x is in [a, b] (both included)
 * @param x the number to check
 * @param a the lower bound of the interval
 * @param b the upper bound of the interval
 * @return 1 if x in [a, b], 0 otherwise
 */
int isInBetween(int x, int a, int b);

/**
 * @brief Unlink all of the shared memory, pipe and semaphore
 */
void unlinkAll();

/**
 * @brief Generate a random 32 bit number based  the 32-bit PCG random number generator
 * homepage : https://www.pcg-random.org/
 * documentation : https://www.pcg-random.org/using-pcg-c.html
 * @param rng_ptr a pointer to the 32-bit pgc
 * @return a number in [0, 2^32[
 */
uint32_t getRandom32bitNumber(pcg32_random_t *rng_ptr);

/**
 * @brief Generate a random pair of coordinates in [0, 6]² based on the 32-bit PCG random number generator
 * @param coords the pair of number to initialize
 * @param rng_ptr a pointer to the 32-bit pgc
 * @param bound the upper bound
 */
void getRandomCoords(int *coords, pcg32_random_t *rng_ptr, int bound);

#endif //PROJET_EPIDEMIE_UTILS_H
