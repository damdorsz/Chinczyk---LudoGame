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
    qDebug() << "AI rzuca kostką, wynik:" << diceValue;
    QVector<Pawn*> playablePawns = mGame->getPlayablePawns(diceValue);

    if (playablePawns.isEmpty()) {
        qDebug() << "Brak dostępnych pionków do ruchu. AI kończy turę.";
        return nullptr;
    }

    Pawn* selectedPawn = selectBestMove(playablePawns, diceValue);
    if (selectedPawn == nullptr) {
        qDebug() << "AI nie znalazło pionka do ruchu. Tura kończy się.";
        return nullptr;
    }

    qDebug() << "AI wybrało pionek:" << (int)selectedPawn->getColor();
    return selectedPawn;
}

Pawn* GameAI::selectBestMove(const QVector<Pawn*>& pawns, int diceValue) {

    for (Pawn* pawn : pawns) {
        if (pawn->isAtHome() && diceValue == 6) {
            qDebug() << "AI wychodzi z domku pionkiem:" << (int )pawn->getColor();
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
                    qDebug() << "AI zbija pionka przeciwnika:" << (int ) targetPawn->getColor();
                    return pawn; // Znaleziono pionek do zbicia
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
        qDebug() << "AI wybiera pionka najbliżej mety:" << (int ) bestPawn->getColor();
        return bestPawn;
    }

    qDebug() << "AI wybiera pierwszy dostępny pionek:" << (int )pawns.first()->getColor();
    return pawns.first();
}

