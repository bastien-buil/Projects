/**
 * @file define.h
 * @brief file with most of the macro used with the programs
 * @author Jasmine Chaid-Akacem <jasmine.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */

#ifndef PROJET_EPIDEMIE_DEFINE_H
#define PROJET_EPIDEMIE_DEFINE_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Aliases of signals for clarity
 */
#define START_OF_TURN SIGALRM
#define END_OF_TURN SIGUSR1
#define CITIZEN_HANDLING_COMMAND SIGUSR1
#define END_OF_GAME SIGUSR2

/**
 * Aliases of the pipe and queue channel
 */
#define PIDEPIDEMIC_CHANNEL "pidEpidemicTimerChannel"
#define CITIZEN_CHANNEL "citizenChannel"
#define PRESSAGENCY_CHANNEL "pressAgencyChannel"
#define TIMER_CHAHNNEL "timerChannel"
#define PRESS_CHANNEL "/pressChannel"

/**
 * Aliases to files and mode of opening
 */
#define PATH_TO_EVOLUTION "evolution.txt"
#define PATH_TO_NEWS_LOG "newsLog.txt"
#define O_FAPPEND "a"

/**
 * Aliases of shared memory
 */
#define MEMTOWN "/board"
#define SEMTOWN "/sem_town"
#define SEMSTATS "/sem_stats"
#define MEMSTATS "/stats"

/**
 * Aliases of numbers used in the press agency
 */
#define JOURNALIST_CONTAMINATION 1
#define CITY_CONTAMINATION 5
#define DEATH_TOLL 10
#define CONTAMINATION_TOLL 2

#endif //PROJET_EPIDEMIE_DEFINE_H
