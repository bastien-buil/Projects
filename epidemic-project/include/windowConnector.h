/**
 * @file windowConnector
 * @brief file with functions to create all the communication with the window
 * @author Jasmine Chaid-Akacem <jasmine.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */

#ifndef PROJET_EPIDEMIE_WINDOWCONNECTOR_H
#define PROJET_EPIDEMIE_WINDOWCONNECTOR_H

#include "mainWindow.h"

/**
 * @brief Connect pipes to epidemicSim to exchange pid as timer
 * pid is stored in windowInfo
 * @param windowInfo
 */
void connectPipes(windowinfo* windowInfo);

/**
 * @brief Load Town and Statistic shared memories
 * They are stored in windowInfo, you are responsible to close the descriptors and unmap the memory
 * @param windowInfo
 */
void loadSharedMemory(windowinfo* windowInfo);

/**
 * Setup signals handler to intercept END_OF_TURN and END_OF_GAME
 * @param windowInfo
 */
void setupSignal(windowinfo* windowInfo);

/**
 * Send START_OF_TURN signal to the process with the pid windowInfo->epidemicPid
 * @param windowInfo
 */
void sendNewTurn(windowinfo* windowInfo);

/**
 * Load file containing the news log
 * @param windowInfo
 */
void openNewsLog(windowinfo* windowInfo);

/**
 * Load new texts from the news log and display them with the function, from mainWindow, addNews
 * @param windowInfo
 */
void readLog(windowinfo* windowInfo);

/**
 * Close shared memory and file opened by openNewsLog and loadSharedMemory
 * @param windowInfo
 */
void closeSharedMemoryAndFiles(windowinfo* windowInfo);
#endif //PROJET_EPIDEMIE_WINDOWCONNECTOR_H
