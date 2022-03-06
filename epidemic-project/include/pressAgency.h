/**
 * @file pressAgency.h
 * @brief This file contains functions related to the handling of messages
 * @author Jasmine Chaid-Akacem <jasmine.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */

#ifndef PROJET_EPIDEMIE_PRESS_AGENCY_H
#define PROJET_EPIDEMIE_PRESS_AGENCY_H
#include <mqueue.h>

/**
 * @brief prepare for closing the program by doing free, unlink etc.
 */
void prepareClosingPressAgency();

/**
 * @brief Read the message received from the queue
 * @param pressChannel the queue to receive the message from
 */
void readAMessage(mqd_t pressChannel);

#endif //PROJET_EPIDEMIE_PRESS_AGENCY_H
