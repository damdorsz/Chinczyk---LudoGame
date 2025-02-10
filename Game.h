#ifndef GAME_H
#define GAME_H

#include <QRandomGenerator>
#include <QVector>
#include <QPoint>
#include <QString>
#include <QMessageBox>
#include "PaintHelper.h"
#include "PlayerColor.h"

class Pawn;
class Board;

class Game {
public:
    static constexpr bool SIX_FOR_HOME = true;

    Game(unsigned int players, const QVector<PlayerColor>& playerColors, const QVector<QString>& playerModes);
    virtual ~Game();

    int rollDice();
    Board* getGameBoard() const;
    QVector<Pawn*> getPlayablePawns(int diceFace) const;
    unsigned int predictRel(Pawn* p, unsigned int diceFace) const;
    bool playMove(Pawn* pawn, int diceFace);
    PlayerColor getCurrentPlayer() const;
    void changeCurrentPlayer();
    QVector<PlayerColor> getCurrentPlayerSequence() const;
    unsigned int getLastDiceValue() const;
    bool playerHasFinished(PlayerColor p) const;
    bool isFinished() const;
    bool isMoveValid(Pawn* pawn, const QPoint& predictedPosition) const;
    bool isMoveOnOwnPawn(Pawn* movingPawn, int predictedPosition) const;
    QVector<Pawn*> getPawnsOfPlayer(PlayerColor color) const;
    void movePawn(Pawn* pawn, unsigned int diceFace);
    void endGame(PlayerColor winner);
    bool hasPlayerWon(PlayerColor color) const;
    void nextPlayer();
    bool isValidEndZoneMove(Pawn* pawn, int diceFace) const;
    QRect getHomePosition(Pawn* pawn) const;
    int findPawnNumber(Pawn* pawn) const;
    bool isNearForbiddenZone(const QPoint& position, int diceValue, PlayerColor playerColor) const;
    bool checkVictoryConditions(PlayerColor color) const;
    void announceVictory(PlayerColor color);
    void handleNoMoves();
    bool wouldCollideWithSameColor(Pawn* pawn, int diceFace) const;
    bool isCurrentPlayerAI() const;

private:
    bool isPositionOnBoard(const QPoint& position) const;

    unsigned int playersCount{};
    Board* mBoard;
    QVector<PlayerColor> *currentSequence{};
    QVector<QString> playerModes{};
    unsigned int currentPlayerIndex{};
    unsigned int lastDiceValue{};
    QRandomGenerator random{};
};

#endif // GAME_H



