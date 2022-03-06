#include "FileComparator.h"


#include <string>
#include <iostream>
#include <QDirIterator>
#include <QString>
#include "File.h"
#include "Hashing.h"
#include "Bdd.h"
#include "Game.h"
//#include <windows.h>
#include <QStandardPaths>
#include <QMessageBox>

//TODO do a GameCheckState or a thing like that, that contain all information about the game/ enhance game update


// save ./saves/
// dir choosen by the user

// UpdateList -> UpdateSave
// OlderList -> UpdateDir

enum class SyncDirection:int{updateSave=1, updateDir=-1, noUpdate=0, unknownUpdate=-2};



//0 no change, 1 dir is newer, -1 save is newer, -2 unknown
SyncDirection isNewer(QList<QString> updatedList, QList<QString> olderList, QList<QString> unknownList) {//0 no change, 1 dir is newer, -1 save is newer
    if (updatedList.length() == 0 && olderList.length() == 0 && unknownList.length() == 0) {
        return SyncDirection::noUpdate;
    }
    if (olderList.length() == 0 && unknownList.length() == 0) {//TO CHANGE
        return SyncDirection::updateSave;//save is older, update save
    }
    else if (updatedList.length() == 0 && unknownList.length() == 0) {//TO CHANGE
        return SyncDirection::updateDir;//dir is older, update dir
    }
    else
        return SyncDirection::unknownUpdate;

}

//(updatedList, olderList, unknownList, isEmpty)
std::tuple<QList<QString>, QList<QString>, QList<QString>, bool> checkPath(Game &game, Bdd &db)
{
    QList<QString> updatedList;
    QList<QString> olderList;
    QList<QString> unknownList;

    QDateTime lastGlobalGameUpdate = db.getGameLastUpdate(game.getName());

    QList<File> list = db.getPath(game.getName());
    if (list.length() == 0) { 
        return std::tuple<QList<QString>, QList<QString>, QList<QString>, bool>(updatedList, olderList, unknownList, true);
    }
    QDirIterator it(game.getPath(), QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath(it.next());
        QFile file(filePath);

        File f(filePath.mid(game.getPath().length()), fileChecksum(file).toHex(), file.fileTime(QFileDevice::FileModificationTime).toTimeSpec(Qt::UTC));

        int pos = list.indexOf(f);
        if (pos != -1) {
            int result = File::isNewer(f, list.at(pos));
            if (result == 1) {
                updatedList.append(f.getPath());
            }
            else if (result == -1) {
                olderList.append(filePath);
            }
            list.removeAt(pos);
        }
        else {
            if (f.getDate() > lastGlobalGameUpdate) {//New File in save created after last global sync
                updatedList.append(f.getPath());
            }
            else if (game.getLastUpdate() > f.getDate()) {
                olderList.append(f.getPath());
            }
            else {
                unknownList.append(f.getPath());
            }
        }
    }
    while (!list.isEmpty()) {
        File file = list.takeFirst();
        if (file.getDate() <= game.getLastUpdate()) {//File removed in dir after local sync containing the last version of the file
            updatedList.append(file.getPath());
        }
        else {
            olderList.append(file.getPath());     
        }
    }

    return std::tuple<QList<QString>, QList<QString>, QList<QString>, bool> (updatedList, olderList, unknownList, false);

}




//syncDirection: 1 update save, -1 update dir
bool syncFiles(SyncDirection syncDirection, Game game, Bdd& db) {
    if (syncDirection == SyncDirection::updateSave) {
        QList<File> list = db.getPath(game.getName());
        QDirIterator it(game.getPath(), QDir::Files, QDirIterator::Subdirectories);

        while (it.hasNext()) {
            QString filePath(it.next());
            QFile file(filePath);

            File f(filePath.mid(game.getPath().length()), fileChecksum(file).toHex(), file.fileTime(QFileDevice::FileModificationTime).toTimeSpec(Qt::UTC));
           
            int pos = list.indexOf(f);
            if (pos != -1) {
                if (f.getHash() != list.at(pos).getHash()) {
                    QFile saveFile("./saves/" + game.getName() + f.getPath());
                    bool rnSuccess = saveFile.rename(saveFile.fileName() + ".old");
                    if (rnSuccess) {
                        QFile::remove("./saves/" + game.getName() + f.getPath());
                        bool cpSuccess = file.copy("./saves/" + game.getName() + f.getPath());
                        if (cpSuccess) {
                            saveFile.remove();
                            db.addPath(game.getName(), f);
                        }

                    }
                }
                list.removeAt(pos);
            }
            else {
                QFileInfo fi("./saves/" + game.getName() + f.getPath());
                QDir dir;
                dir.mkpath(fi.absolutePath());
                QFile::remove("./saves/" + game.getName() + f.getPath());//Should not be here but for securiy
                bool success = file.copy("./saves/" + game.getName() + f.getPath());
                if (success)
                {
                    db.addPath(game.getName(), f);
                };
            }
        }
        while (!list.isEmpty()){
            File file = list.takeFirst();
            db.removePath(game.getName(), file);
            QFile::remove("./saves/" + game.getName() + file.getPath());           
        }
        return true;
    }
    else if (syncDirection == SyncDirection::updateDir) {
            QList<File> list = db.getPath(game.getName());
            QDirIterator it(game.getPath(), QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                QString filePath(it.next());
                QFile file(filePath);

                File f(filePath.mid(game.getPath().length()), fileChecksum(file).toHex(), file.fileTime(QFileDevice::FileModificationTime).toTimeSpec(Qt::UTC));

                int pos = list.indexOf(f);
                if (pos != -1) {
                    if(f.getHash() != list.at(pos).getHash()){
                        bool rnSuccess = file.rename(file.fileName() + ".old");
                        if (rnSuccess) {
                            bool cpSuccess = QFile::copy("./saves/" + game.getName() + f.getPath(), filePath);
                            if (cpSuccess) {
                                file.remove();
                            }

                        }
                    }
                    list.removeAt(pos);

                }
                else {
                    file.remove();
                }
            }
            while (!list.isEmpty()) {
                File file = list.takeFirst();
                QFileInfo fi(game.getPath() + file.getPath());
                QDir dir;
                dir.mkpath(fi.absolutePath());
                QFile saveFile("./saves/" + game.getName() + file.getPath());
                QFile::remove(game.getPath() + file.getPath());
                saveFile.copy(game.getPath() + file.getPath());
            }
            return true;
        

    }
    return false;


}

//Return success 
bool updateGame(QString gameName, Bdd &db) {
    QString computerId = getComputerId();
    Game game = db.getGame(gameName, computerId);
    std::tuple<QList<QString>, QList<QString>, QList<QString>, bool> tp = checkPath(game, db);
    SyncDirection result;
    if(std::get<3>(tp)){
        result = SyncDirection::updateSave;
    }
    else {
        result = isNewer(std::get<0>(tp), std::get<1>(tp), std::get<2>(tp));
    }
    if (result == SyncDirection::noUpdate) {
        return true;
    }
    else if (result == SyncDirection::updateSave || result == SyncDirection::updateDir) { 
        bool success = syncFiles(result, game, db);
        if (success) {
            db.updateGameDate(gameName, computerId, QDateTime::currentDateTime());
        }
        else {
            QMessageBox::information(nullptr, "Error in update for " + gameName,"");
        }
        return success;
    }
    else {
        return false;
    }


}

/*
DATE SYNC LOCALE PAR PC

SYNC DATE ET ON COMPARE DATE MODIF DU FICHIER AVEC SYNC DATE
SI UN FICHIER NOUVEAU CREER APRES SYNC BA ON LE RECUP


SI UN FICHIER MANQUANT, ET QUE LA DATE DE MODIF DANS LA SAVE EST PLUS VIELLE QUE DERNIERE SYNC LOCALE
-> ON MET A JOUR LA SAVE

*/


#include <QSysInfo>
#include <QTextCodec>

QString getComputerId() {
    /*HW_PROFILE_INFO hwProfileInfo;
    if (GetCurrentHwProfile(&hwProfileInfo))
        return QString::fromWCharArray(hwProfileInfo.szHwProfileGuid); doesn't support update
        */
    return  QString(QSysInfo::machineUniqueId());;
}