/**
 * @file mainWindow.h
 * @brief file with the functions associated with the graphical interface
 * @author Jasmine Chaid-Akacem <jasmine.chaid-akacem@ecole.ensicaen.fr>
 * @author Bastien Buil <bastien.buil@ecole.ensicaen.fr>
 * @author Sonia Hammami <sonia.hammami@ecole.ensicaen.fr>
 * @author Hugo Buffler <hugo.buffler@ecole.ensicaen.fr>
 */

#ifndef PROJET_EPIDEMIE_MAINWINDOW_H
#define PROJET_EPIDEMIE_MAINWINDOW_H


#include <gtk/gtk.h>
#include "epidemicSim.h"


typedef struct {
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* dayTextLabel;
    GtkWidget* playNextTurnLabel;
    GtkWidget* mapsDraw;
    GtkWidget* legends;
    GtkWidget* image;

    GtkWidget* frameNews;
    GtkWidget* scrollNews;
    GtkWidget* listNews;

    GtkWidget* viewportNews;

    GtkWidget* frameStat;
    GtkWidget* gridStat;

    GtkWidget* viewPortStat;

} windowwidgets;

typedef struct {
    GdkPixbuf *citizen;
    GdkPixbuf *journalist;
    GdkPixbuf *doctor;
    GdkPixbuf *firefighter;
    GdkPixbuf *burned;
    GdkPixbuf *dead;
    GdkPixbuf *citizenLegend;
    GdkPixbuf *journalistLegend;
    GdkPixbuf *doctorLegend;
    GdkPixbuf *firefighterLegend;
    GdkPixbuf *burnedLegend;
    GdkPixbuf *deadLegend;

    GdkPixbuf *wastelandLegend;
    GdkPixbuf *hospitalLegend;
    GdkPixbuf *stationLegend;
    GdkPixbuf *houseLegend;



    int spriteSize;
} sprites_t;


typedef struct {
    gboolean roundOnGoing;
    windowwidgets widgets;
    sprites_t sprites;
    int turn_number;
    town *_town;
    statistics *townStats;
    int epidemicPid;
    FILE* logFile;
    int fdStats;
    int fdCity;

} windowinfo;

/**
 * @brief Update the display of the stats, the number of the day
 * @param windowInfo
 */
void displayStatsAndDay(windowinfo* windowInfo);

/**
 * @brief Load all sprite to windowInfo.sprites, the sprite are resize to size x size
 * @param windowInfo
 * @param size of the sprite
 */
void loadSprites(windowinfo* windowInfo, int size);

/**
 * @brief Unload sprites in windowInfo.sprites
 * @param windowInfo
 */
void unloadSprites(windowinfo* windowInfo);

/**
 * @brief Set the image of the graph (it is used to display or update the image)
 * @param windowInfo
 */
void updateImage(windowinfo* windowInfo);

/**
 * @brief Add a new news to the list of news (and scroll at the top of the list)
 * @param windowInfo
 * @param news String containing the news
 */
void addNews(windowinfo* windowInfo, char* news);
#endif //PROJET_EPIDEMIE_MAINWINDOW_H
