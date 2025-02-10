#ifndef PATH_H
#define PATH_H

#include "PlayerColor.h"
#include <string>

#include <QPoint>
#include <QVector>

class Path {
public:
    static const unsigned int MAX_REL;
    static QPoint getAbsoluteCordinates(unsigned int rel);
    static QPoint rotatePointToRight(QPoint p);
    static QPoint rotatePointToRight(QPoint p, int times);
    static QPoint getEndZoneCordinates(unsigned int rel);


private:
    static QVector<QPoint> endZone;
    static QVector<QPoint> path;
    static const int TOP = 10;
};

#endif //Path.h
