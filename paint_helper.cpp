#include <paint_helper.h>

#include <PlayerColor.h>
#include <Pawn.h>
#include <GameWindow.h>
#include <ValueError.h>
#include <Board.h>

#include <QDebug>

namespace painthelp {

    //Return the absolute rectangular area on screen of the cell by it's x and y cordinates
    QRect getCellRect(int x, int y) {
        return QRect(
            GameWindow::BOARD_BOUND + (x * GameWindow::CELL_SIZE),
            GameWindow::BOARD_BOUND + (y * GameWindow::CELL_SIZE),
            GameWindow::CELL_SIZE, GameWindow::CELL_SIZE);
    }

    //Eqvi. to getCellRect(int, int) but uses QPoint as param instead
    QRect getCellRect(QPoint cell) {
        return getCellRect(cell.x(), cell.y());
    }

    //Returns the rectangular region covering the home of red player
    QRect getTopLeftHomeRect() {
        return QRect (
            GameWindow::BOARD_BOUND,
            GameWindow::BOARD_BOUND,
            GameWindow::CELL_SIZE * 4,
            GameWindow::CELL_SIZE * 4
            );
    }

    //Returns the rectangular region covering the home of yellow player
    QRect getTopRightHomeRect() {
        QRect cell = getCellRect(7, 0);
        return QRect (
            cell.x(), cell.y(),
            GameWindow::CELL_SIZE * 4,
            GameWindow::CELL_SIZE * 4
            );
    }

    //Returns the rectangular region covering the home of blue player
    QRect getBottomRightHomeRect() {
        QRect cell = getCellRect(7, 7);
        return QRect (
            cell.x(), cell.y(),
            GameWindow::CELL_SIZE * 4,
            GameWindow::CELL_SIZE * 4
            );
    }

    //Returns the rectangular region covering the home of green player
    QRect getBottomLeftHomeRect() {
        QRect cell = getCellRect(0, 7);
        return QRect (
            cell.x(), cell.y(),
            GameWindow::CELL_SIZE * 4,
            GameWindow::CELL_SIZE * 4
            );
    }

    //Returns the rectangular region as QRect covering the destination box
    QRect getDestRect() {
        QRect cell = getCellRect(5, 5);
        return QRect (
            cell.x(), cell.y(),
            GameWindow::CELL_SIZE * 1,
            GameWindow::CELL_SIZE * 1
        );
    }

    //Shifts the rect's corner to center of itself
    QRect shiftToCenter(QRect it) {
        return QRect (
            it.x() + (GameWindow::CELL_SIZE / 2),
            it.y() + (GameWindow::CELL_SIZE / 2),
            GameWindow::CELL_SIZE,
            GameWindow::CELL_SIZE
        );
    }

    //Returns the vector of rects where white circles inside homes are to be drawn
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

    QRect getPawnHomePosGeometry(PlayerColor color, int which, int numerGracza) {
        // Walidacja parametrów wejściowych
        if (which < 1 || which > 4) {
            qDebug() << "Invalid which value:" << which;
            which = 1; // Bezpieczna wartość domyślna
        }

        if (numerGracza < 1 || numerGracza > 4) {
            qDebug() << "Invalid numerGracza value:" << numerGracza;
            numerGracza = 1; // Bezpieczna wartość domyślna
        }

        QVector<QRect> v = getHomeCircleRects();

        // Mapowanie pozycji dla każdego gracza
        int baseIndex = 0;
        switch (numerGracza) {
        case 1: // Lewy górny róg
            baseIndex = 0;
            break;
        case 2: // Prawy dolny róg
            baseIndex = 8;
            break;
        case 3: // Prawy górny róg
            baseIndex = 4;
            break;
        case 4: // Lewy dolny róg
            baseIndex = 12;
            break;
        default:
            qDebug() << "Unexpected numerGracza value:" << numerGracza;
            return v[0]; // Bezpieczna wartość domyślna
        }

        // Oblicz finalny indeks
        int finalIndex = baseIndex + (which - 1);

        // Dodatkowe zabezpieczenie przed wyjściem poza zakres
        if (finalIndex >= v.size()) {
            qDebug() << "Index out of bounds:" << finalIndex;
            return v[0]; // Bezpieczna wartość domyślna
        }

        // Debug info
        qDebug() << "getPawnHomePosGeometry:"
                 << "color:" << static_cast<int>(color)
                 << "which:" << which
                 << "numerGracza:" << numerGracza
                 << "baseIndex:" << baseIndex
                 << "finalIndex:" << finalIndex;

        return getPawnGeometry(v[finalIndex]);
    }

    // QRect getPawnHomePosGeometry(PlayerColor color, int which,int numerGracza) {
    //     if(which < 1 || which > 4)
    //         ValueError::raise_new(QString("Invalid value for which : %1").arg(which));

    //     QVector<QRect> v = getHomeCircleRects();
    //     switch (color) {
    //         case PlayerColor::RED:
    //             return getPawnGeometry(v[which-1]);
    //         case PlayerColor::YELLOW:
    //             return getPawnGeometry(v[which+3]);
    //         case PlayerColor::BLUE:
    //             return getPawnGeometry(v[which+7]);
    //         case PlayerColor::GREEN:
    //             return getPawnGeometry(v[which+11]);

    //         //This should never happen.
    //         default:
    //             ValueError::raise_new(QString("paint_helper.cpp:getPawnHomePos : \
    //             Critical Error (no enum value fell for a switch)"));
    //     }
    //     #pragma GCC diagnostic ignored "-Wreturn-type"
    // }
    // #pragma GCC diagnostic warning "-Wreturn-type"

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
        return getPawnDestGeometry(p->getColor(),Board::getTabGraczyKolor());
    }

    /* Returns the geometry that a pawn is supposed to have according to the
     * cell it is in right now. Currently, it has same effect as calling 'getCellRect'
     * but MAY differ in future due to modifications. The size of the pawn is
     * handled internally by Pawn class itself */
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

    //Returns the point adjecent to the given point, lieing below it
    QPoint pointBelow(QPoint p) {
        return QPoint {p.x(), p.y()+1};
    }

    //Returns the point adjecent to the given point, lieing right to it
    QPoint pointRight(QPoint p) {
        return QPoint {p.x()+1, p.y()};
    }

    //Returns the point adjecent to the given point, lieing above it
    QPoint pointAbove(QPoint p) {
        return QPoint {p.x(), p.y()-1};
    }

    //Returns the point adjecent to the given point, lieing left to it
    QPoint pointLeft(QPoint p) {
        return QPoint {p.x()-1, p.y()};
    }

    //Returns the QRect on the cell where guider dots are to be drawn
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
