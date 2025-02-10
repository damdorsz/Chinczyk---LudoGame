#ifndef GAMEAI_H
#define GAMEAI_H

#include "Game.h"
#include "Pawn.h"
#include <QThread>
#include <QDebug>



class GameAI
{
public:
    GameAI(Game* game);
    Pawn* makeMove(int diceValue);

private:
    Pawn* selectBestMove(const QVector<Pawn*>& pawns, int diceValue);
    Game* mGame;
};

#endif // GAMEAI_H
