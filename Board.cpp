#include <Board.h>

#include <QString>
#include <QDebug>

#include <Pawn.h>
#include <PlayerColor.h>
#include <Path.h>
#include <ValueError.h>

QVector<PlayerColor> Board::mPlayersColours = {PlayerColor::WHITE, PlayerColor::WHITE, PlayerColor::WHITE, PlayerColor::WHITE};

QVector<PlayerColor> Board::getPlayersColours()
{
    return mPlayersColours;
}

void Board::initializePlayerPawns(PlayerColor color, unsigned int startPosition, unsigned int playerNumber) {
    for (unsigned int i = 0; i < PAWNS_PER_PLAYER; ++i) {
        mPawns.append(new Pawn(color, startPosition + i, playerNumber));
    }
    mPlayersColours[playerNumber - 1] = color;
}

void Board::validatePlayerCount(unsigned int players) const {
    if (players < MIN_PLAYERS || players > MAX_PLAYERS) {
        ValueError::raise_new(QString("Invalid number of players. Expected value between %1 to %2 but got %3")
                                  .arg(MIN_PLAYERS)
                                  .arg(MAX_PLAYERS)
                                  .arg(players));
    }
}

Board::Board(unsigned int players,const QVector<PlayerColor>& PlayersColours)
    : players_count(players) {
    validatePlayerCount(players);
    if (players >= 4) initializePlayerPawns(PlayersColours[3], 0, 4);
    if (players >= 3) initializePlayerPawns(PlayersColours[2], 4, 3);
    if (players >= 2) {
        initializePlayerPawns(PlayersColours[1], 8, 2);
        initializePlayerPawns(PlayersColours[0], 12, 1);
    }
}

Board::~Board() {
    for (auto p : mPawns)
        delete p;
}

unsigned int Board::getPlayersCount() const {
    return players_count;
}

QPoint Board::getPawnCoordinates(PlayerColor color,unsigned int relpos) {
    qInfo() << "Board::getPawnCoordinates(PlayerColor, int) : relpos == " << relpos;

    if (color == mPlayersColours[0]) {
        return Path::getAbsoluteCordinates(relpos);
    } else if (color == mPlayersColours[2]) {
        return Path::rotatePointToRight(Path::getAbsoluteCordinates(relpos));
    } else if (color == mPlayersColours[1]) {
        return Path::rotatePointToRight(Path::getAbsoluteCordinates(relpos), 2);
    } else if (color == mPlayersColours[3]) {
        return Path::rotatePointToRight(Path::getAbsoluteCordinates(relpos), 3);
    } else {
        ValueError::raise_new(QString("Board::getPawnCoordinates(PlayerColor, int) : Invalid PlayerColor"));
    }
}

QPoint Board::getPawnEndZone(PlayerColor color,unsigned int relpos) {
    relpos -= 40;
    qInfo() << "Board::getPawnEndZone(PlayerColor, int) : relpos == " << relpos;
    if (color == mPlayersColours[0]) {
        return Path::getEndZoneCordinates(relpos);
    } else if (color == mPlayersColours[2]) {
        return Path::rotatePointToRight(Path::getEndZoneCordinates(relpos));
    } else if (color == mPlayersColours[1]) {
        return Path::rotatePointToRight(Path::getEndZoneCordinates(relpos), 2);
    } else if (color == mPlayersColours[3]) {
        return Path::rotatePointToRight(Path::getEndZoneCordinates(relpos), 3);
    } else {
        ValueError::raise_new(QString("Board::getPawnEndZone(PlayerColor, int) : Invalid PlayerColor"));
    }
}


QPoint Board::getPawnCoordinates(Pawn* p) {
    return getPawnCoordinates(p->getColor(), p->getRelPosition());
}

QVector<Pawn*> Board::getAllPawns() const {
    return mPawns;
}

QVector<Pawn*> Board::getPawnsAt(const QPoint& point) const {
    qInfo() << "Board::getPawnsAt(QPoint)";

    QVector<Pawn*> result {};
    for (auto pawn : mPawns) {
        if(pawn->isAtHome())
            continue;
        if(getPawnCoordinates(pawn).x() == point.x() && getPawnCoordinates(pawn).y() == point.y())
            result.append(pawn);
    }

    return result;
}

Pawn* Board::getPawnById(unsigned int id) const {
    return mPawns[id];
}

QVector<Pawn*> Board::getAllPawnsByColor(PlayerColor color) const {
    if(color == PlayerColor::YELLOW && players_count == 2)
        ValueError::raise_new(QString("Board::getAllPawnsByColor : Current game does not have YELLOW color"));
    if(color == PlayerColor::GREEN && players_count != 4)
        ValueError::raise_new(QString("Board::getAllPawnsByColor : Current game does not have GREEN color"));

    QVector<Pawn*> result {};
    for (auto p : mPawns) {
        if(p->getColor() == color)
            result.append(p);
    }

    return result;
}
