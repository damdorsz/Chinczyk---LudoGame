#ifndef GAMEAI_H
#define GAMEAI_H

#include "Game.h"
#include "Pawn.h"
#include <QThread>  // Zamień QThread na QTimer, bo używamy QTimer::singleShot
#include <QDebug>  // Do logowania



class GameAI   // Dziedziczymy po QObject, żeby móc używać QTimer
{
public:
    GameAI(Game* game);  // Dodajemy explicit i parent
    Pawn* makeMove(int diceValue);

private:
    Pawn* selectBestMove(const QVector<Pawn*>& pawns, int diceValue);
    Game* mGame;
};

#endif // GAMEAI_H
