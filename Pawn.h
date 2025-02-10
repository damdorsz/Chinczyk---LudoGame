#ifndef PAWN_H
#define PAWN_H

#include <QPoint>
#include <QRect>
#include <QWidget>
#include <QPaintEvent>
#include <QString>
#include <QPropertyAnimation>

#include "PlayerColor.h"

class Board;
class GameWindow;
class SaveGameEngine;
enum class PlayerColor;

class Pawn : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal glow READ getGlow WRITE setGlow)
public:

    static const int HOME = -1;
    static const int DEST = 43;
    static const int GLOW_DURATION = 700;
    static const qreal GLOW_SCALE;
    Pawn(PlayerColor color, int id,int position);
    virtual ~Pawn();
    void attatchWindow(GameWindow *w);
    int getRelPosition();
    PlayerColor getColor();
    unsigned int getId();
    unsigned int getIndex();
    QPoint getPositionOnBoard();
    bool isAtHome();
    bool hasReachedDestination();
    void changePosition(int rel);
     void moveForward(int steps);
    void goBackHome(bool clash = true);
    void setGlow(qreal glow);
    qreal getGlow();
    void setEnabled(bool enabled);

signals:
    void visualClicked(Pawn* p);
    void clashed(Pawn *p);

private:
    PlayerColor mColor {};
    unsigned int mId {};
    int mPos {};
    qreal mGlow {0};

    QPropertyAnimation *glowEffect {nullptr};

    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
};

#endif //Pawn.h
