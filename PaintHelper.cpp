#include "PaintHelper.h"

#include <PlayerColor.h>
#include <Pawn.h>
#include <GameWindow.h>
#include <ValueError.h>
#include <Board.h>

#include <QDebug>

namespace painthelp {
QRect getCellRect(int x, int y) {
    return QRect(
        GameWindow::BOARD_BOUND + (x * GameWindow::CELL_SIZE),
        GameWindow::BOARD_BOUND + (y * GameWindow::CELL_SIZE),
        GameWindow::CELL_SIZE, GameWindow::CELL_SIZE);
}

QRect getCellRect(QPoint cell) {
    return getCellRect(cell.x(), cell.y());
}

QRect getTopLeftHomeRect() {
    return QRect (
        GameWindow::BOARD_BOUND,
        GameWindow::BOARD_BOUND,
        GameWindow::CELL_SIZE * 4,
        GameWindow::CELL_SIZE * 4
        );
}

QRect getTopRightHomeRect() {
    QRect cell = getCellRect(7, 0);
    return QRect (
        cell.x(), cell.y(),
        GameWindow::CELL_SIZE * 4,
        GameWindow::CELL_SIZE * 4
        );
}

QRect getBottomRightHomeRect() {
    QRect cell = getCellRect(7, 7);
    return QRect (
        cell.x(), cell.y(),
        GameWindow::CELL_SIZE * 4,
        GameWindow::CELL_SIZE * 4
        );
}

QRect getBottomLeftHomeRect() {
    QRect cell = getCellRect(0, 7);
    return QRect (
        cell.x(), cell.y(),
        GameWindow::CELL_SIZE * 4,
        GameWindow::CELL_SIZE * 4
        );
}

QRect getDestRect() {
    QRect cell = getCellRect(5, 5);
    return QRect (
        cell.x(), cell.y(),
        GameWindow::CELL_SIZE * 1,
        GameWindow::CELL_SIZE * 1
        );
}

QRect shiftToCenter(QRect it) {
    return QRect (
        it.x() + (GameWindow::CELL_SIZE / 2),
        it.y() + (GameWindow::CELL_SIZE / 2),
        GameWindow::CELL_SIZE,
        GameWindow::CELL_SIZE
        );
}

QVector<QRect> getHomeCircleRects() {
    return QVector<QRect> {
        shiftToCenter(getCellRect(0, 0)),
        shiftToCenter(getCellRect(0, 2)),
        shiftToCenter(getCellRect(2, 0)),
        shiftToCenter(getCellRect(2, 2)),

        shiftToCenter(getCellRect(7, 0)),
        shiftToCenter(getCellRect(9, 0)),
        shiftToCenter(getCellRect(7, 2)),
        shiftToCenter(getCellRect(9, 2)),

        shiftToCenter(getCellRect(7, 7)),
        shiftToCenter(getCellRect(9, 7)),
        shiftToCenter(getCellRect(7, 9)),
        shiftToCenter(getCellRect(9, 9)),

        shiftToCenter(getCellRect(0, 7)),
        shiftToCenter(getCellRect(0, 9)),
        shiftToCenter(getCellRect(2, 7)),
        shiftToCenter(getCellRect(2, 9)),
    };
}

QRect getPawnHomePosGeometry(PlayerColor color, int which, int playerNumber) {

    if (which < 1 || which > 4) {
        qDebug() << "Invalid which value:" << which;
        which = 1;
    }

    if (playerNumber < 1 || playerNumber > 4) {
        qDebug() << "Invalid numerGracza value:" << playerNumber;
        playerNumber = 1;
    }

    QVector<QRect> v = getHomeCircleRects();

    int baseIndex = 0;
    switch (playerNumber) {
    case 1:
        baseIndex = 0;
        break;
    case 2:
        baseIndex = 8;
        break;
    case 3:
        baseIndex = 4;
        break;
    case 4:
        baseIndex = 12;
        break;
    default:
        qDebug() << "Unexpected numerGracza value:" << playerNumber;
        return v[0];
    }

    int finalIndex = baseIndex + (which - 1);


    if (finalIndex >= v.size()) {
        qDebug() << "Index out of bounds:" << finalIndex;
        return v[0];
    }

    qDebug() << "getPawnHomePosGeometry:"
             << "color:" << static_cast<int>(color)
             << "which:" << which
             << "player number:" << playerNumber
             << "baseIndex:" << baseIndex
             << "finalIndex:" << finalIndex;

    return getPawnGeometry(v[finalIndex]);
}

QRect getPawnDestGeometry(PlayerColor color, QVector<PlayerColor> colorPlayer) {
    QPoint cell;
    if(color == colorPlayer[0])
    {
        cell = {4, 5};
    } else if(color == colorPlayer[1])
    {
        cell = {6, 5};
    } else if(color == colorPlayer[2])
    {
        cell = {5, 4};
    }else if(color == colorPlayer[3])
    {
        cell = {5, 6};
    }
    return getCellRect(QPoint(cell));
}

QRect getPawnDestGeometry(Pawn *p) {
    return getPawnDestGeometry(p->getColor(),Board::getPlayersColours());
}

QRect getPawnGeometry(QRect cellRect) {
    return {
        cellRect.x(),
        cellRect.y(),
        GameWindow::CELL_SIZE, GameWindow::CELL_SIZE
    };
}

QRect getPawnGeometry(int x, int y) {
    return getPawnGeometry(getCellRect(x, y));
}

QRect getPawnGeometry(QPoint pos) {
    return getPawnGeometry(pos.x(), pos.y());
}

QRect getPawnGeometry(Pawn *p) {
    return getPawnGeometry(p->getPositionOnBoard());
}

QPoint pointBelow(QPoint p) {
    return QPoint {p.x(), p.y()+1};
}

QPoint pointRight(QPoint p) {
    return QPoint {p.x()+1, p.y()};
}

QPoint pointAbove(QPoint p) {
    return QPoint {p.x(), p.y()-1};
}

QPoint pointLeft(QPoint p) {
    return QPoint {p.x()-1, p.y()};
}

QRect getGuiderRegion(QRect cell) {

    QPoint center = cell.center();

    qDebug() << cell.x() << cell.y();
    qDebug() << center.x() << cell.y();

    QPoint tl = {
        center.x() - ((GameWindow::GUIDER_DOT_SIZE / 2) + (GameWindow::STROKE_WIDTH * 2)),
        center.x() - ((GameWindow::GUIDER_DOT_SIZE / 2) + (GameWindow::STROKE_WIDTH * 2))
    };
    QPoint br = {
        center.x() + ((GameWindow::GUIDER_DOT_SIZE / 2) + (GameWindow::STROKE_WIDTH * 2)),
        center.x() + ((GameWindow::GUIDER_DOT_SIZE / 2) + (GameWindow::STROKE_WIDTH * 2))
    };

    qDebug() << cell.x() << cell.y();
    qDebug() << center.x() << center.y();
    return QRect {tl, br};
}

}
