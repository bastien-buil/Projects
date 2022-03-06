#include <gtk/gtk.h>
#include <sys/wait.h>
#include "citizenManager.h"
#include "mainWindow.h"
#include "windowConnector.h"
#include "epidemicSim.h"

#define NB_RESIDENT_PER_ROW_IN_CASE 4
#define LEGEND_SPRITE_SIZE 20

/**
 * @brief Get the color to represent the cell
 * @param cell that we want to represent
 * @return a GdkRGBA with the color for the cell
 */
GdkRGBA getColorByCaseType(cellType cell){
    GdkRGBA color;
    color.alpha = 1;
    switch (cell) {
        case wasteland:
            color.red = 0.294;
            color.green  = 0.612;
            color.blue  = 0.404;
            break;
        case hospital:
            color.red = 1;
            color.green  = 1;
            color.blue  = 1;
            break;
        case station:
            color.red = 1;
            color.green  = 0;
            color.blue  = 0;
            break;
        default:
        case house:
            color.red = 0.671;
            color.green  = 0.329;
            color.blue  = 0.329;
            break;
    }
    return color;
}

/**
 * @brief Get the sprite to represent an inhabitant
 * @param windowinfo
 * @param resident
 * @return a pointer to GdkPixbuf containing the sprite
 */
GdkPixbuf* chooseSprite(windowinfo* windowinfo, inhabitants* resident){
    GdkPixbuf *pixHab;
    if(resident->condition == dead){
        return windowinfo->sprites.dead;
    }
    if(resident->condition == burned){
        return windowinfo->sprites.burned;
    }
    switch (resident->job) {
        case fireFighter:
            pixHab = windowinfo->sprites.firefighter;
            break;
        case journalist:
            pixHab = windowinfo->sprites.journalist;
            break;
        case doctor:
            pixHab = windowinfo->sprites.doctor;
            break;
        case citizen:
            pixHab = windowinfo->sprites.citizen;
            break;
        default:
            pixHab = windowinfo->sprites.citizen;
    }
    return pixHab;
}


/**
 * @brief Call to draw the GtkDrawingArea with the information
 * @param widget the GtkDrawingArea in which we will draw
 * @param cr a cairo context used to draw in the GtkDrawingArea
 * @param data a pointer to a windowinfo
 * @return
 */
gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data){
    guint width, height;
    GdkRGBA color;
    GtkStyleContext *context;


    context = gtk_widget_get_style_context (widget);
    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);


    windowinfo* windowinfo = data;

    gtk_render_background (context, cr, 0, 0, width, height);

    unsigned int boxSize = (width / WIDTH < height / HEIGHT) ?  width / WIDTH : height / HEIGHT;
    unsigned int marginLeft = (width - boxSize * WIDTH)/2;
    unsigned int  marginTop = (height - boxSize * HEIGHT)/2;

    int residentSize = ((int)boxSize-2)/NB_RESIDENT_PER_ROW_IN_CASE;

    if(windowinfo->sprites.spriteSize != residentSize){
        unloadSprites(windowinfo);
        loadSprites(windowinfo, residentSize);
    }

    for(int j = 0; j < HEIGHT; j++){
        for(int i = 0; i< WIDTH; i++){
            cairo_rectangle (cr, marginLeft+boxSize*i, marginTop + boxSize*j, boxSize, boxSize);
            color = getColorByCaseType(windowinfo->_town->board[j][i].type);
            gdk_cairo_set_source_rgba (cr, &color);
            cairo_fill (cr);
            cairo_rectangle (cr, marginLeft + boxSize*i, marginTop + boxSize*j, boxSize, boxSize);
            color.red = 0;
            color.blue = 0;
            color.green = 0;
            color.alpha = 1;
            gdk_cairo_set_source_rgba (cr, &color);
            cairo_stroke(cr);

            for(int iResident = 0; iResident < windowinfo->_town->board[j][i].numberOfOccupants; iResident++){
                int numResident = windowinfo->_town->board[j][i].residents[iResident];
                inhabitants resident = windowinfo->_town->inhabitant[numResident];
                GdkPixbuf *pixHab = chooseSprite(windowinfo, &resident);
                int posXInCase = iResident % NB_RESIDENT_PER_ROW_IN_CASE;
                int posYInCase = iResident / NB_RESIDENT_PER_ROW_IN_CASE;
                gdk_cairo_set_source_pixbuf(cr, pixHab, marginLeft +boxSize*i + residentSize * posXInCase + 1, marginTop + boxSize * j + residentSize * posYInCase + 1);
                cairo_paint(cr);
            }

        }
    }

    return FALSE;
}

/**
 * @brief Call when a key is pressed
 * @param widget (unused) widget that detect the event
 * @param event Event of the key press
 * @param data windowinfo
 * @return If the event is intercepted
 */
gboolean on_keyrelease_event(__attribute__((unused)) GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_space){
        sendNewTurn(data);

        return TRUE;
    }
    return FALSE;
}

void updateImage(windowinfo* windowInfo){
    gtk_image_set_from_file (GTK_IMAGE(windowInfo->widgets.image), "evolution_stats.png");
}

void displayStatsAndDay(windowinfo* windowInfo){
    char persSaine[128];
    char persMalade[128];
    char persMorte[128];
    char persBrule[128];

    char jour[128];

    sprintf(persSaine, "Personnes en vie : %u", windowInfo->_town->nbInhabitantsAlive);
    sprintf(persMalade, "Personnes malades : %u", windowInfo->townStats->numberOfContaminated);
    sprintf(persMorte, "Personnes mortes : %u", windowInfo->townStats->numberOfDeath);
    sprintf(persBrule, "Personnes brulées : %u", windowInfo->townStats->numberOfBurnt);
    sprintf(jour, "Jour %d", windowInfo->turn_number);

    gtk_label_set_text(GTK_LABEL(gtk_grid_get_child_at(GTK_GRID(windowInfo->widgets.gridStat), 0, 0)), persSaine);
    gtk_label_set_text(GTK_LABEL(gtk_grid_get_child_at(GTK_GRID(windowInfo->widgets.gridStat), 0, 1)), persMalade);
    gtk_label_set_text(GTK_LABEL(gtk_grid_get_child_at(GTK_GRID(windowInfo->widgets.gridStat), 0, 2)), persMorte);
    gtk_label_set_text(GTK_LABEL(gtk_grid_get_child_at(GTK_GRID(windowInfo->widgets.gridStat), 0, 3)), persBrule);

    gtk_label_set_text(GTK_LABEL(windowInfo->widgets.dayTextLabel), jour);
    gtk_widget_show_all(windowInfo->widgets.dayTextLabel);
    gtk_widget_show_all(windowInfo->widgets.gridStat);
}

void addNews(windowinfo* windowInfo, char* news){
    GtkWidget* widget = gtk_label_new(news);
    gtk_list_box_insert (GTK_LIST_BOX(windowInfo->widgets.listNews), widget, 0);
    gtk_widget_show_all(widget);
    GtkAdjustment * vAdjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(windowInfo->widgets.scrollNews));
    gtk_adjustment_set_value(vAdjustment, 0);
}

/**
 * Add a
 * @param line the vertical position to which put the line
 * @param name of the legends to display
 * @param image of the element of the legends
 * @param grid in which the line will be put
 */
void addLineToLegend(int line, char* name, GdkPixbuf* image, GtkGrid* grid){
    GtkWidget* doctorLabel = gtk_label_new(name);
    gtk_grid_attach(grid, doctorLabel, 0, line, 1, 1);
    GtkWidget* doctorImage = gtk_image_new_from_pixbuf(image);
    gtk_grid_attach(grid, doctorImage, 1, line, 1, 1);
}

/**
 * @brief Call to display the app interface
 * @param app
 * @param windowInfo
 */
void create_interface(GtkApplication *app, windowinfo* windowInfo){
    windowInfo->widgets.window = gtk_application_window_new (app);

    windowInfo->widgets.grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(windowInfo->widgets.window), windowInfo->widgets.grid);
    gtk_grid_set_column_homogeneous((GtkGrid*)windowInfo->widgets.grid, 1);
    gtk_grid_set_row_homogeneous((GtkGrid*)windowInfo->widgets.grid, 1);

    windowInfo->widgets.dayTextLabel = gtk_label_new("Jour 0");
    gtk_grid_attach((GtkGrid*) windowInfo->widgets.grid, windowInfo->widgets.dayTextLabel, 0, 0, 1, 1);

    windowInfo->widgets.playNextTurnLabel = gtk_label_new("Appuyer sur espace pour passer");
    gtk_grid_attach((GtkGrid*) windowInfo->widgets.grid, windowInfo->widgets.playNextTurnLabel, 0, 1, 1, 1);

    windowInfo->widgets.mapsDraw = gtk_drawing_area_new ();
    //gtk_widget_set_size_request (windowInfo->widgets.mapsDraw, 200, 200);

    //gtk_widget_set_halign (windowInfo->widgets.mapsDraw, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(windowInfo->widgets.mapsDraw, 1);

    g_signal_connect (G_OBJECT (windowInfo->widgets.mapsDraw), "draw",
                      G_CALLBACK (draw_callback), windowInfo);

    gtk_grid_attach((GtkGrid*) windowInfo->widgets.grid, windowInfo->widgets.mapsDraw, 1, 0, 2, 2);


    windowInfo->widgets.legends = gtk_frame_new ("Légende");

    GtkWidget* legendsGrid = gtk_grid_new();

    addLineToLegend(0, "Citoyen", windowInfo->sprites.citizenLegend, (GtkGrid *) legendsGrid);
    addLineToLegend(1, "Docteur", windowInfo->sprites.doctorLegend, (GtkGrid *) legendsGrid);
    addLineToLegend(2, "Pompier", windowInfo->sprites.firefighterLegend, (GtkGrid *) legendsGrid);
    addLineToLegend(3, "Journaliste", windowInfo->sprites.journalistLegend, (GtkGrid *) legendsGrid);
    addLineToLegend(4, "Mort", windowInfo->sprites.deadLegend, (GtkGrid *) legendsGrid);
    addLineToLegend(5, "Brulé", windowInfo->sprites.burnedLegend, (GtkGrid *) legendsGrid);

    addLineToLegend(6, "Terrain vague", windowInfo->sprites.wastelandLegend, (GtkGrid *) legendsGrid);
    addLineToLegend(7, "Maison", windowInfo->sprites.houseLegend, (GtkGrid *) legendsGrid);
    addLineToLegend(8, "Hôpital", windowInfo->sprites.hospitalLegend, (GtkGrid *) legendsGrid);
    addLineToLegend(9, "Caserne", windowInfo->sprites.stationLegend, (GtkGrid *) legendsGrid);

    gtk_container_add(GTK_CONTAINER(windowInfo->widgets.legends), legendsGrid);
    gtk_grid_attach((GtkGrid*) windowInfo->widgets.grid, windowInfo->widgets.legends, 3, 0, 1, 2);


    windowInfo->widgets.image = gtk_image_new();
    gtk_grid_attach((GtkGrid*) windowInfo->widgets.grid, windowInfo->widgets.image, 0, 2, 2, 2);


    windowInfo->widgets.frameNews = gtk_frame_new ("News");
    windowInfo->widgets.listNews = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(windowInfo->widgets.listNews), GTK_SELECTION_NONE);
    windowInfo->widgets.scrollNews = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(windowInfo->widgets.frameNews), windowInfo->widgets.scrollNews);
    gtk_container_add(GTK_CONTAINER(windowInfo->widgets.scrollNews), windowInfo->widgets.listNews);


    gtk_grid_attach((GtkGrid*) windowInfo->widgets.grid, windowInfo->widgets.frameNews, 2, 2, 2, 1);

    windowInfo->widgets.frameStat = gtk_frame_new ("Stats");
    windowInfo->widgets.gridStat = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(windowInfo->widgets.gridStat), gtk_label_new(NULL), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(windowInfo->widgets.gridStat), gtk_label_new(NULL), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(windowInfo->widgets.gridStat), gtk_label_new(NULL), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(windowInfo->widgets.gridStat), gtk_label_new(NULL), 0, 3, 1, 1);

    gtk_container_add(GTK_CONTAINER(windowInfo->widgets.frameStat), windowInfo->widgets.gridStat);
    gtk_grid_attach((GtkGrid*) windowInfo->widgets.grid, windowInfo->widgets.frameStat, 2, 3, 2, 1);

    gtk_widget_add_events((GtkWidget *) windowInfo->widgets.window, GDK_KEY_PRESS_MASK);
    g_signal_connect(G_OBJECT(windowInfo->widgets.window), "key_release_event", G_CALLBACK(on_keyrelease_event), windowInfo);

}

/**
 * @brief Called when the app is created
 * @param app
 * @param data
 */
static void on_activate (GtkApplication *app, gpointer data) {
    windowinfo* windowInfo = data;
    create_interface(app, windowInfo);
    addNews(windowInfo, "Ouverture de la connexion à la presse");
    gtk_widget_show_all (windowInfo->widgets.window);
    openNewsLog(windowInfo);
}


/**
 * Create a GdkPixbuf from an image path, and resize it to square of size x size
 * @param spritePath to the image
 * @param size of the Pixbuf created
 * @return a pointer to a GdkPixbuf of the image (you are responsible to unref it)
 */
GdkPixbuf* loadSprite(char* spritePath, int size){
    GdkPixbuf *pixSprite;
    GdkPixbuf *pixSpriteResized;
    GError *err = NULL;
    pixSprite = gdk_pixbuf_new_from_file(spritePath, &err);
    if(err)
    {
        printf("Error in loading img %s: %s\n", spritePath, err->message);
        g_error_free(err);
        return FALSE;
    }
    pixSpriteResized = gdk_pixbuf_scale_simple(pixSprite, size, size, GDK_INTERP_TILES);
    g_object_unref(pixSprite);
    return pixSpriteResized;
}

/**
 * Create an image containing a square of color
 * @param color of the square
 * @param size of the square
 * @return a pointer to a GdkPixbuf of the image (you are responsible to unref it)
 */
GdkPixbuf* createSquareSurfaceFromColor(GdkRGBA color, int size){
    GdkPixbuf* colorSquare = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, size, size);
    guint32 colorPixel = (guint32) (color.red * 255) << 24 |
            (guint32) (color.green * 255) << 16 |
            (guint32) (color.blue * 255) << 8 |
            (guint32) (color.alpha * 255);

    gdk_pixbuf_fill(colorSquare, colorPixel);
    return colorSquare;
}

void loadSprites(windowinfo* windowInfo, int size){
    windowInfo->sprites.spriteSize = size;
    windowInfo->sprites.citizen = loadSprite("sprites/citizen.png", size);
    windowInfo->sprites.journalist = loadSprite("sprites/journalist.png", size);
    windowInfo->sprites.doctor = loadSprite("sprites/doctor.png", size);
    windowInfo->sprites.firefighter = loadSprite("sprites/firefighter.png", size);
    windowInfo->sprites.dead = loadSprite("sprites/dead.png", size);
    windowInfo->sprites.burned = loadSprite("sprites/burned.png", size);
}

/**
 * @brief Load the sprites of the legends
 * @param windowInfo
 */
void loadLegendSprites(windowinfo* windowInfo){
    windowInfo->sprites.citizenLegend = loadSprite("sprites/citizen.png", LEGEND_SPRITE_SIZE);
    windowInfo->sprites.journalistLegend = loadSprite("sprites/journalist.png", LEGEND_SPRITE_SIZE);
    windowInfo->sprites.doctorLegend = loadSprite("sprites/doctor.png", LEGEND_SPRITE_SIZE);
    windowInfo->sprites.firefighterLegend = loadSprite("sprites/firefighter.png", LEGEND_SPRITE_SIZE);
    windowInfo->sprites.deadLegend = loadSprite("sprites/dead.png", LEGEND_SPRITE_SIZE);
    windowInfo->sprites.burnedLegend = loadSprite("sprites/burned.png", LEGEND_SPRITE_SIZE);

    windowInfo->sprites.wastelandLegend = createSquareSurfaceFromColor(getColorByCaseType(wasteland), LEGEND_SPRITE_SIZE);
    windowInfo->sprites.hospitalLegend = createSquareSurfaceFromColor(getColorByCaseType(hospital), LEGEND_SPRITE_SIZE);
    windowInfo->sprites.houseLegend = createSquareSurfaceFromColor(getColorByCaseType(house), LEGEND_SPRITE_SIZE);
    windowInfo->sprites.stationLegend = createSquareSurfaceFromColor(getColorByCaseType(station), LEGEND_SPRITE_SIZE);

}

/**
 * Unload the sprite of the map
 * @param windowInfo
 */
void unloadSprites(windowinfo* windowInfo){
    g_object_unref(windowInfo->sprites.citizen);
    g_object_unref(windowInfo->sprites.journalist);
    g_object_unref(windowInfo->sprites.doctor);
    g_object_unref(windowInfo->sprites.firefighter);
    g_object_unref(windowInfo->sprites.dead);
    g_object_unref(windowInfo->sprites.burned);
}

/**
 * Unload the sprites of the legend
 * @param windowInfo
 */
void unloadLegendSprites(windowinfo* windowInfo){
    g_object_unref(windowInfo->sprites.citizenLegend);
    g_object_unref(windowInfo->sprites.journalistLegend);
    g_object_unref(windowInfo->sprites.doctorLegend);
    g_object_unref(windowInfo->sprites.firefighterLegend);
    g_object_unref(windowInfo->sprites.deadLegend);
    g_object_unref(windowInfo->sprites.burnedLegend);

    g_object_unref(windowInfo->sprites.wastelandLegend);
    g_object_unref(windowInfo->sprites.hospitalLegend);
    g_object_unref(windowInfo->sprites.houseLegend);
    g_object_unref(windowInfo->sprites.stationLegend);
}

int main (int argc, char *argv[]) {
    windowinfo windowInfo;
    windowInfo.turn_number = 0;
    windowInfo.roundOnGoing = FALSE;
    connectPipes(&windowInfo);
    loadSharedMemory(&windowInfo);
    setupSignal(&windowInfo);

    loadSprites(&windowInfo, 28);
    loadLegendSprites(&windowInfo);

    GtkApplication *app = gtk_application_new ("fr.ensicaen.projetSystEpidemie",
                                             G_APPLICATION_FLAGS_NONE);
    g_set_application_name("Projet Epidemie");
    g_signal_connect (app, "activate", G_CALLBACK (on_activate), &windowInfo);
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    unloadSprites(&windowInfo);
    unloadLegendSprites(&windowInfo);
    closeSharedMemoryAndFiles(&windowInfo);
    g_object_unref(app);
    return status;
}


