/**
 * @file timer.h
 * @brief This file contains the definition of the alarm's method used to alert epidemicSim
 * @author Jasmine Chaid-Akacem <jasmine.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */
#ifndef PROJET_EPIDEMIE_TIMER_H
#define PROJET_EPIDEMIE_TIMER_H
#include "utils.h"

/**
 * @brief this function send a signal to epidemicSim at each end of turn
 * @param sig a signal parameter needed for the function
 */
void alarmEpidemicSim(__attribute__((unused)) int sig);

#endif //PROJET_EPIDEMIE_TIMER_H
