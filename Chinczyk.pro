QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    About.cpp \
    Board.cpp \
    Dice.cpp \
    Game.cpp \
    GameScreen.cpp \
    GameWindow.cpp \
    Path.cpp \
    Pawn.cpp \
    SaveGameEngine.cpp \
    ValueError.cpp \
    gameplaySettings.cpp \
    main.cpp \
    paint_helper.cpp

HEADERS += \
    About.h \
    Board.h \
    Dice.h \
    Game.h \
    GameScreen.h \
    GameWindow.h \
    Path.h \
    Pawn.h \
    PlayerColor.h \
    SaveGameEngine.h \
    ValueError.h \
    gameplaySettings.h \
    paint_helper.h

FORMS += \
    about.ui \
    gameplaSettings.ui \
    welcome.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
