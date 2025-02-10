#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <QObject>
#include <QPaintEvent>
#include <QWidget>
#include <QVector>

#include <PlayerColor.h>
#include <PaintHelper.h>

class GameWindow;

class GameScreen : public QWidget {
Q_OBJECT;

public:
    explicit GameScreen(GameWindow *parent,const QVector<PlayerColor>& coloursPlayers);

    virtual ~GameScreen();

private:
    void drawGuidePaths(QPainter &painter);
    QColor whichColour(PlayerColor searchColour);
    void drawHomes(QPainter &painter);
    QVector<PlayerColor> coloursPlayers;

protected:
    virtual void paintEvent(QPaintEvent*);
};

#endif
