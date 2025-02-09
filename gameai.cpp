#include "gameai.h"
#include <Board.h>
#include <Pawn.h>
#include <PlayerColor.h>
#include <QTimer>
#include "Game.h"

GameAI::GameAI(Game* game)
    :
    mGame(game)
{
}

void GameAI::makeMove(int diceValue) {
    qDebug() << "AI rzuca kostką, wynik:" << diceValue;
    QVector<Pawn*> playablePawns = mGame->getPlayablePawns(diceValue);
    qDebug() << "AI rzuca kostką, wynik:" << diceValue;
    if (playablePawns.isEmpty()) {
        qDebug() << "Brak dostępnych pionków do ruchu. AI kończy turę.";
        // Dodajemy małe opóźnienie przed zmianą gracza
        QTimer::singleShot(500, [this]() {
            mGame->changeCurrentPlayer();
        });
        return;
    } else qDebug() << "Dostępne pionki na plansze.";

    // Wybierz najlepszy ruch według strategii
    Pawn* selectedPawn = selectBestMove(playablePawns, diceValue);
    if (selectedPawn == nullptr) {
        qDebug() << "Nie znaleziono pionka do ruchu.";
        QTimer::singleShot(500, [this]() {
            mGame->changeCurrentPlayer();
        });
        return;
    } else qDebug() << "Znaleziono pionka do ruchu.";

    // Wykonaj ruch
    bool reroll = mGame->playMove(selectedPawn, diceValue);
    qDebug() << "AI wykonuje ruch pionkiem:" << (int)selectedPawn->getColor();

    // Jeśli dostajemy dodatkowy ruch, wykonaj go po małym opóźnieniu
    if (reroll) {
        QTimer::singleShot(1000, [this, diceValue]() {
            makeMove(diceValue);
        });
    } else {
        // Zmiana gracza po małym opóźnieniu
        QTimer::singleShot(500, [this]() {
            mGame->changeCurrentPlayer();
        });
    }
}

Pawn* GameAI::selectBestMove(const QVector<Pawn*>& pawns, int diceValue) {
    // 1. Jeśli możemy wyjść z domku (wyrzucono 6), zrób to
    for (Pawn* pawn : pawns) {
        if (pawn->isAtHome() && diceValue == 6) {
            qDebug() << "AI wychodzi z domku pionkiem:" << (int )pawn->getColor();
            return pawn;
        }
    }

    // 2. Jeśli możemy zbić pionek przeciwnika, zrób to
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

    // 3. Priorytetyzuj pionki najbliżej mety
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

    // 4. Jeśli żadna z powyższych strategii nie zadziałała, wybierz pierwszy dostępny pionek
    qDebug() << "AI wybiera pierwszy dostępny pionek:" << (int )pawns.first()->getColor();
    return pawns.first();
}
