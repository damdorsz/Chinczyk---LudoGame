#ifndef GAME_H
#define GAME_H

#include <QRandomGenerator>
#include <QVector>
#include <QPoint>
#include <PlayerColor.h>
#include "PaintHelper.h"
#include <QMessageBox>


class Pawn;
class Board;
enum class PlayerColor;

class Game {

public:
    static constexpr bool SIX_FOR_HOME = true;
    Game(unsigned int players,QVector<PlayerColor> tablicaKolorowGraczy,QVector<QString> tablicaModeGamers);
    virtual ~Game();

    int rollDice();
    Board* getGameBoard();
    QVector<Pawn*> getPlayablePawns(int diceFace);
    unsigned int predictRel(Pawn* p,unsigned int diceFace);
    bool playMove(Pawn* pawn, int diceFace);
    PlayerColor getCurrentPlayer();
    void changeCurrentPlayer();
    QVector<PlayerColor> getCurrentPlayerSequence();
    unsigned int getLastDiceValue();
    bool playerHasFinished(PlayerColor p);
    bool isFinished();
    bool isMoveValid(Pawn* pawn, const QPoint& predictedPosition);
    bool isMoveOnOwnPawn(Pawn *movingPawn, int predictedPosition);
    QVector<Pawn*> getPawnsOfPlayer(PlayerColor color);
    void movePawn(Pawn* pawn, unsigned int diceFace);
    void endGame(PlayerColor winner);
    bool hasPlayerWon(PlayerColor color);
    void nextPlayer();
    bool isValidEndZoneMove(Pawn* pawn, int diceFace);
    QRect getHomePosition(Pawn* pawn);
    int findPawnNumber(Pawn* pawn);
    bool isNearForbiddenZone(const QPoint& position, int diceValue, PlayerColor playerColor);
    bool checkVictoryConditions(PlayerColor color);
    void announceVictory(PlayerColor color);
    void handleNoMoves();
    bool wouldCollideWithSameColor(Pawn* pawn, int diceFace);
    bool isCurrentPlayerAI();

private:

    bool isPositionOnBoard(const QPoint& position);
    unsigned int players_count {};
    Board* mBoard {};
    QVector<PlayerColor> *currentSequence {};
    QVector<QString> mPlayerModes {};
    unsigned int current {};
    unsigned int lastDiceValue {};
    QRandomGenerator random {};
};

#endif //Game.h



