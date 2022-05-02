QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./include \
    ./include/models \
    ./include/presenters \
    ./include/views/iconsdrawers \
    ./include/views/viewcomponents \
    ./include/views

DEPENDPATH += ./include \
    ./include/models \
    ./include/presenters \
    ./include/views/iconsdrawers \
    ./include/views/viewcomponents \
    ./include/views

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/models/cell.cpp \
    src/models/gamemodel.cpp \
    src/models/grid.cpp \
    src/models/gridparser.cpp \
    src/models/linestate.cpp \
    src/presenters/command.cpp \
    src/presenters/gameoptionsdialog.cpp \
    src/presenters/gamepresenter.cpp \
    src/presenters/gridreader.cpp \
    src/presenters/icondrawertype.cpp \
    src/presenters/timer.cpp \
    src/views/gameview.cpp \
    src/views/gameviewstate.cpp \
    src/views/iconsdrawers/customiconsdrawer.cpp \
    src/views/iconsdrawers/roundicondrawer.cpp \
    src/views/viewcomponents/gameviewcomponent.cpp \
    src/views/viewcomponents/gridcomponent.cpp \
    src/views/viewcomponents/infopanelcomponent.cpp \
    src/views/viewcomponents/lineinfocomponent.cpp

HEADERS += \
    include/mainwindow.h \
    include/models/cell.h \
    include/models/gamemodel.h \
    include/models/grid.h \
    include/models/gridparser.h \
    include/models/linestate.h \
    include/presenters/command.h \
    include/presenters/gameoptionsdialog.h \
    include/presenters/gamepresenter.h \
    include/presenters/gridreader.h \
    include/presenters/icondrawertype.h \
    include/presenters/timer.h \
    include/views/gameview.h \
    include/views/gameviewstate.h \
    include/views/iconsdrawers/customiconsdrawer.h \
    include/views/iconsdrawers/icondrawer.h \
    include/views/iconsdrawers/roundicondrawer.h \
    include/views/viewcomponents/gameviewcomponent.h \
    include/views/viewcomponents/gridcomponent.h \
    include/views/viewcomponents/infopanelcomponent.h \
    include/views/viewcomponents/lineinfocomponent.h

FORMS += \
    gameoptionsdialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    ProjetTakuzu_fr-FR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressources.qrc
