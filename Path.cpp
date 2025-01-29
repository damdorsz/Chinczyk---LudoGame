#include <Path.h>

#include <QString>

#include <ValueError.h>

QVector<QPoint> Path::path = {
    QPoint(0,4),
    QPoint(1,4),
    QPoint(2,4),
    QPoint(3,4),
    QPoint(4,4),
    QPoint(4,3),
    QPoint(4,2),
    QPoint(4,1),
    QPoint(4,0),
    QPoint(5,0),
    QPoint(6, 0),
    QPoint(6, 1),
    QPoint(6, 2),
    QPoint(6, 3),
    QPoint(6, 4),
    QPoint(7, 4),
    QPoint(8, 4),
    QPoint(9, 4),
    QPoint(10, 4),
    QPoint(10, 5),
    QPoint(10, 6),
    QPoint(9, 6),
    QPoint(8, 6),
    QPoint(7, 6),
    QPoint(6, 6),
    QPoint(6, 7),
    QPoint(6, 8),
    QPoint(6, 9),
    QPoint(6, 10),
    QPoint(5, 10),
    QPoint(4, 10),
    QPoint(4, 9),
    QPoint(4, 8),
    QPoint(4, 7),
    QPoint(4, 6),
    QPoint(3, 6),
    QPoint(2, 6),
    QPoint(1, 6),
    QPoint(0, 6),
    QPoint(0, 5),
    QPoint(1, 5),
    QPoint(2, 5),
    QPoint(3, 5),
    QPoint(4, 5)
};

const unsigned int Path::MAX_REL = Path::path.size() - 1;

QPoint Path::getAbsoluteCordinates(unsigned int rel) {
    if(rel >= (unsigned int)path.size())
        ValueError::raise_new(QString("Path::getAbsoluteCordinates(unsigned long int) \
        : Invalid argument : rel == %1").arg(rel));

    return path[rel];
}

QPoint Path::rotatePointToRight(QPoint point) {
    return QPoint {TOP-point.y(), point.x()};
};

QPoint Path::rotatePointToRight(QPoint point, int times) {
    if (times == 0)
        return point;
    return rotatePointToRight(rotatePointToRight(point), times-1);
};
