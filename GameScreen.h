#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <QObject>
#include <QPaintEvent>
#include <QWidget>
#include <QVector>

#include <PlayerColor.h>
#include <paint_helper.h>

class GameWindow;

class GameScreen : public QWidget {
Q_OBJECT;

public:
    explicit GameScreen(GameWindow *parent,QVector<PlayerColor> tablicaGraczyKolor);

    virtual ~GameScreen();

private:
    void drawHomes(QPainter &painter);
    void drawGuidePaths(QPainter &painter);
    QColor whichColour(PlayerColor szukanyKolor);
    QVector<PlayerColor> tabGraczyKolor;

protected:
    virtual void paintEvent(QPaintEvent*);
};

#endif
