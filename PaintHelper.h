#ifndef PAINTHELPER_H
#define PAINTHELPER_H

#include <QVector>
#include <QString>
#include <QRect>
#include <QPoint>
#include "PlayerColor.h"

class Pawn;

namespace painthelp {
QRect getCellRect(int x, int y);
QRect getCellRect(QPoint cell);
QRect getBottomRightHomeRect();
QRect getTopRightHomeRect();
QRect getTopLeftHomeRect();
QRect getBottomLeftHomeRect();
QRect getDestRect();
QRect shiftToCenter(QRect it);
QVector<QRect> getHomeCircleRects();
QRect getPawnHomePosGeometry(PlayerColor color, int which, int playerNumber);
QRect getPawnDestGeometry(PlayerColor,QVector<PlayerColor> colorPlayer);
QRect getPawnDestGeometry(Pawn*);
QRect getPawnGeometry(QRect cellRect);
QRect getPawnGeometry(int x, int y);
QRect getPawnGeometry(QPoint);
QRect getPawnGeometry(Pawn *p);
QPoint pointBelow(QPoint p);
QPoint pointRight(QPoint p);
QPoint pointAbove(QPoint p);
QPoint pointLeft(QPoint p);
QRect getGuiderRegion(QRect cell);
}

#endif // PAINTHELPER_H
