#include "GameAI.h"
#include <Board.h>
#include <Pawn.h>
#include <PlayerColor.h>
#include "Game.h"

#include <QTimer>

GameAI::GameAI(Game* game)
    :
    mGame(game)
{
}

Pawn* GameAI::makeMove(int diceValue) {
    qDebug() << "AI throws the dice, the result:" << diceValue;
    QVector<Pawn*> playablePawns = mGame->getPlayablePawns(diceValue);

    if (playablePawns.isEmpty()) {
        qDebug() << "No pawns available to move. AI ends the turn.";
        return nullptr;
    }

    Pawn* selectedPawn = selectBestMove(playablePawns, diceValue);
    if (selectedPawn == nullptr) {
        qDebug() << "AI has not found a pawn to move. The turn ends.";
        return nullptr;
    }

    qDebug() << "AI has chosen the pawn:" << (int)selectedPawn->getColor();
    return selectedPawn;
}

Pawn* GameAI::selectBestMove(const QVector<Pawn*>& pawns, int diceValue) {

    for (Pawn* pawn : pawns) {
        if (pawn->isAtHome() && diceValue == 6) {
            qDebug() << "AI exits the house with a pawn:" << (int )pawn->getColor();
            return pawn;
        }
    }

    for (Pawn* pawn : pawns) {
        if (!pawn->isAtHome()) {
            unsigned int futurePos = mGame->predictRel(pawn, diceValue);
            QPoint futureCoord = mGame->getGameBoard()->getPawnCoordinates(pawn->getColor(), futurePos);
            QVector<Pawn*> pawnsAtTarget = mGame->getGameBoard()->getPawnsAt(futureCoord);

            for (Pawn* targetPawn : pawnsAtTarget) {
                if (targetPawn->getColor() != pawn->getColor()) {
                    qDebug() << "AI knocks down the opponent's pawn:" << (int ) targetPawn->getColor();
                    return pawn;
                }
            }
        }
    }

    Pawn* bestPawn = nullptr;
    int highestPosition = -1;

    for (Pawn* pawn : pawns) {
        if (!pawn->isAtHome() && !pawn->hasReachedDestination()) {
            int currentPos = pawn->getRelPosition();
            if (currentPos > highestPosition) {
                highestPosition = currentPos;
                bestPawn = pawn;
            }
        }
    }

    if (bestPawn) {
        qDebug() << "AI chooses the pawn closest to the finish line:" << (int ) bestPawn->getColor();
        return bestPawn;
    }

    qDebug() << "AI selects the first available pawn:" << (int )pawns.first()->getColor();
    return pawns.first();
}

