/**
* @file main.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

/**
 * @brief main Entry point of the application
 * Installs a french translator if needed and executes the window
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList languages = QLocale().uiLanguages();
    if(languages.size()){
        QTranslator *translator = new QTranslator;
        QString language = languages.front();
        translator->load(QString(":/translation/ProjetTakuzu_%1.qm").arg(language));
        a.installTranslator(translator);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
