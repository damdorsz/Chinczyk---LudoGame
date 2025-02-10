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
    GameAI.cpp \
    GameScreen.cpp \
    GameWindow.cpp \
    GameplaySettings.cpp \
    PaintHelper.cpp \
    Path.cpp \
    Pawn.cpp \
    ValueError.cpp \
    main.cpp

HEADERS += \
    About.h \
    Board.h \
    Dice.h \
    Game.h \
    GameAI.h \
    GameScreen.h \
    GameWindow.h \
    GameplaySettings.h \
    PaintHelper.h \
    Path.h \
    Pawn.h \
    PlayerColor.h \
    ValueError.h

FORMS += \
    GameplaySettings.ui \
    about.ui \
    welcome.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
