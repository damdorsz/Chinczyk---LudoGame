#ifndef BOARD_H
#define BOARD_H

#include <string>

#include <QVector>
#include <QPoint>

#include "PlayerColor.h"

class Pawn;

class Board {
public:
    explicit Board(unsigned int players, QVector<PlayerColor>& PlayersColours);
    virtual ~Board();

    unsigned int getPlayersCount() const;
    static QPoint getPawnCoordinates(PlayerColor color, unsigned int relpos)  ;
    static QPoint getPawnCoordinates(Pawn *p) ;
    QVector<Pawn*> getAllPawns() const;
    QVector<Pawn*> getPawnsAt(const QPoint& point) const;
    Pawn* getPawnById(unsigned int id) const;
    QVector<Pawn*> getAllPawnsByColor(PlayerColor color) const;
    static QVector<QPoint> getEndZone(PlayerColor color);
    static QVector<QPoint> endZone;
    Pawn* getPawnAtPosition(const QPoint& position) const;
    static QVector<PlayerColor> getPlayersColours();
    static QPoint getPawnEndZone(PlayerColor color, unsigned int relpos);
    void initializePlayerPawns(PlayerColor color, unsigned int startPosition, unsigned int playerNumber);
    void validatePlayerCount(unsigned int players) const;

private:
    static const unsigned int MIN_PLAYERS = 2;
    static const unsigned int MAX_PLAYERS = 4;
    static const unsigned int PAWNS_PER_PLAYER = 4;
    unsigned int players_count {};
    QVector<Pawn*> mPawns {};
    static QVector<PlayerColor> mPlayersColours;
};

#endif // BOARD_H
