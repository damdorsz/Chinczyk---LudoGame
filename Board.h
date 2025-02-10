#ifndef BOARD_H
#define BOARD_H

#include <string>

#include <QVector>
#include <QPoint>

#include "PlayerColor.h"

class Pawn;

class Board {
public:

    Board(unsigned int players, QVector<PlayerColor> tablicaGraczyKolor);
    virtual ~Board();

    unsigned int getPlayersCount();
    static QPoint getPawnCoordinates(PlayerColor color, unsigned int relpos);
    static QPoint getPawnCoordinates(Pawn* p);
    QVector<Pawn*> getAllPawns();
    QVector<Pawn*> getPawnsAt(QPoint point);
    Pawn* getPawnById(unsigned int id);
    QVector<Pawn*> getAllPawnsByColor(PlayerColor color);
    static QVector<QPoint> getEndZone(PlayerColor color);
    static QVector<QPoint> endZone;
    Pawn* getPawnAtPosition(const QPoint& position);
    static QVector<PlayerColor> getTabGraczyKolor();
    static QPoint getPawnEndZone(PlayerColor color,unsigned int relpos);
    void initializePlayerPawns(PlayerColor color, unsigned int startPosition, unsigned int playerNumber);
    void validatePlayerCount(unsigned int players);
private:
    static const unsigned int MIN_PLAYERS = 2;
    static const unsigned int MAX_PLAYERS = 4;
    static const unsigned int PAWNS_PER_PLAYER = 4;
    unsigned int players_count {};
    QVector<Pawn*> mPawns {};
    static QVector<PlayerColor> mTablicaGraczyKolor;

};

#endif //Board.h
