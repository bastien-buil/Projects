#include "SyncGaming.h"
#include <QtWidgets/QApplication>

#include <QMessageBox>
//#include "sqlite/sqlite3.h"
#include "Bdd.h"


int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    Bdd db = Bdd();
    db.createTables();
    db.close();


    SyncGaming w;
    w.show();

    return a.exec();
}
