#include <Game.h>

#include <QDebug>

#include <Board.h>
#include <Pawn.h>
#include <PlayerColor.h>
#include <SaveGameEngine.h>
#include <ValueError.h>

Game::Game(unsigned int players, QVector<PlayerColor> tablicaKolorowGraczy) :
    players_count(players), mBoard(new Board {players,tablicaKolorowGraczy}),
    currentSequence(new QVector<PlayerColor> {}), current (0),
    random (QRandomGenerator::securelySeeded()) {

    switch(players) {
    case 2:
        currentSequence->push_back(tablicaKolorowGraczy[0]);
        currentSequence->push_back(tablicaKolorowGraczy[1]); //So that pawns are at opposite corners
        break;
    case 3:
        currentSequence->push_back(tablicaKolorowGraczy[0]);
        currentSequence->push_back(tablicaKolorowGraczy[1]);
        currentSequence->push_back(tablicaKolorowGraczy[2]);
        break;
    case 4:
        currentSequence->push_back(tablicaKolorowGraczy[0]);
        currentSequence->push_back(tablicaKolorowGraczy[1]);
        currentSequence->push_back(tablicaKolorowGraczy[2]);
        currentSequence->push_back(tablicaKolorowGraczy[3]);
        break;
    }

    // for (size_t i = 0; i < players; ++i) {
    //     currentSequence->push_back(tablicaKolorowGraczy[i]);
    // }
}

// Game::Game(unsigned int players) : // alfa 1.0
//     players_count(players), mBoard(new Board {players}),
//     currentSequence(new QVector<PlayerColor> {}), current (0),
//     random (QRandomGenerator::securelySeeded()) {

//     switch(players) {
//         case 2:
//             currentSequence->push_back(PlayerColor::RED);
//             currentSequence->push_back(PlayerColor::BLUE); //So that pawns are at opposite corners
//             break;
//         case 3:
//             currentSequence->push_back(PlayerColor::RED);
//             currentSequence->push_back(PlayerColor::YELLOW);
//             currentSequence->push_back(PlayerColor::BLUE);
//             break;
//         case 4:
//             currentSequence->push_back(PlayerColor::RED);
//             currentSequence->push_back(PlayerColor::YELLOW);
//             currentSequence->push_back(PlayerColor::BLUE);
//             currentSequence->push_back(PlayerColor::GREEN);
//             break;
//     }
// }

Game::Game(SaveGameEngine *save) {
    this->players_count = save->readInt();
    
    this->currentSequence = new QVector<PlayerColor> {};
    for(unsigned int i = 0; i < players_count; i++)
        this->currentSequence->append(static_cast<PlayerColor>(save->readInt()));
    
    this->current = save->readInt();
    this->lastDiceValue = save->readInt();
    
    this->mBoard = save->getBoard();
    
    this->random = QRandomGenerator::securelySeeded();
}

void Game::serializeInto(SaveGameEngine *save) {
    save->writeInt(this->players_count);
    
    for(auto p : *(this->currentSequence))
        save->writeInt(static_cast<int>(p));
    
    save->writeInt(this->current);
    save->writeInt(this->lastDiceValue);
}

Game::~Game() {
    delete currentSequence;
    delete mBoard;
}

int Game::rollDice() {
    //TODO:: Generate a random number between 1 to 6

    lastDiceValue = random.bounded(1, 7);
    return lastDiceValue;
}

PlayerColor Game::getCurrentPlayer() {
    return (*currentSequence)[current];
}

Board* Game::getGameBoard() {
    return mBoard;
}

unsigned int Game::predictRel(Pawn* pawn, unsigned int diceFace) {
    qInfo() << "Game::predictRel(Pawn*, int)";

    // Najpierw sprawdź wszystkie warunki
    if(pawn->isAtHome() && diceFace != 6 && SIX_FOR_HOME) {
        ValueError::raise_new(QString("Invalid move: need 6 to leave home"));
    }

    unsigned int newPosition;
    if(pawn->isAtHome()) {
        newPosition = pawn->getRelPosition() + 1;
    } else {
        newPosition = pawn->getRelPosition() + diceFace;
    }

    if(newPosition > Pawn::DEST) {
        ValueError::raise_new(QString("Invalid move: would exceed board limit"));
    }

    return newPosition;
}

// unsigned int Game::predictRel(Pawn* pawn, unsigned int diceFace) { // alfa 1.0
//     qInfo() << "Game::predictRel(Pawn*, int)";

//     if(pawn->isAtHome() && diceFace != 6 && SIX_FOR_HOME)
//         ValueError::raise_new(QString("Invalid move"));
//     if(pawn->getRelPosition() + diceFace > Pawn::DEST)
//         ValueError::raise_new(QString("Invalid move"));

//     if(pawn->isAtHome()) //Just get out of home by one step
//         return pawn->getRelPosition() + 1;
//     else
//         return pawn->getRelPosition() + diceFace;
// }

void Game::changeCurrentPlayer() {
    if(current >= (unsigned int)(currentSequence->size())-1) {
        current = 0;
        return;
    }

    current++;
}

QVector<PlayerColor> Game::getCurrentPlayerSequence() {
    return *currentSequence;
}

unsigned int Game::getLastDiceValue() {
    return lastDiceValue;
}

bool Game::playerHasFinished(PlayerColor c) {
    for(auto p : mBoard->getAllPawnsByColor(c))
        if(!p->hasReachedDestination())
            return false;

    return false;
}

bool Game::isFinished() {
    unsigned int i = 0;
    for (auto c : *currentSequence)
        if(playerHasFinished(c))
            i++;

    return i >= players_count - 1;
}

bool Game::playMove(Pawn* pawn, int diceFace) {
    qInfo() << "Game::playMove(Pawn*, int)";

    if(pawn->getRelPosition() + diceFace > Pawn::DEST) {
        qDebug() << "Prevented move that would exceed board limit";
        return false;
    }

    // Sprawdź czy ruch jest w ogóle możliwy
    QVector<Pawn*> playablePawns = getPlayablePawns(diceFace);
    if (playablePawns.isEmpty()) {
        handleNoMoves();
        return false;
    }

    // Sprawdź czy wybrany pionek jest wśród możliwych do ruchu
    if (!playablePawns.contains(pawn)) {
        qDebug() << "Wybrany pionek nie może wykonać ruchu";
        return false;
    }

    unsigned int futureRel = Game::predictRel(pawn, diceFace);
    bool re_turn = diceFace == 6;
    Pawn* toClash = nullptr;

    if(futureRel != Pawn::DEST) {
        QVector<Pawn*> pawnsThere = mBoard->getPawnsAt(
            mBoard->getPawnCoordinates(
                pawn->getColor(),
                futureRel
                )
            );
        if(pawnsThere.size() == 1 && pawnsThere[0]->getColor() != pawn->getColor()) {
            toClash = pawnsThere[0];
            re_turn = true;
        }
    } else {
        re_turn = true;
    }

    // Wykonaj ruch
    pawn->changePosition(futureRel);

    // Sprawdź warunki zwycięstwa
    if (checkVictoryConditions(pawn->getColor()))
            announceVictory(pawn->getColor());


    if(toClash != nullptr)
        toClash->goBackHome();

    // Emituj sygnał o wykonanym ruchu (przydatne dla AI)
    //emit moveMade(pawn, diceFace);

    return re_turn;
}

// Dodajmy nową metodę sprawdzającą kolizje
bool Game::wouldCollideWithSameColor(Pawn* pawn, int diceFace) {
    QPoint futurePos = mBoard->getPawnCoordinates(
        pawn->getColor(),
        Game::predictRel(pawn, diceFace)
        );

    QVector<Pawn*> pawnsAtTarget = mBoard->getPawnsAt(futurePos);
    for (auto& otherPawn : pawnsAtTarget) {
        if (otherPawn->getColor() == pawn->getColor()) {
            return true;
        }
    }
    return false;
}

// Zmodyfikowana metoda getPlayablePawns
QVector<Pawn*> Game::getPlayablePawns(int diceFace) {
    if(diceFace < 1 || diceFace > 6)
        ValueError::raise_new(QString("Invalid dice value : %1").arg(diceFace));

    QVector<Pawn*> result;
    QVector<Pawn*> pawns = mBoard->getAllPawns();

    for(auto p : pawns) {
        unsigned int currentPos = p->getRelPosition();
        qDebug() << "Checking pawn at position:" << currentPos;
        if(currentPos + diceFace > Pawn::DEST) {
            qDebug() << "Skipping pawn: would exceed DEST (" << Pawn::DEST << ")";
            continue;
        }
        if(p->getColor() != getCurrentPlayer())
            continue;
        if(p->isAtHome() && diceFace != 6 && SIX_FOR_HOME)
            continue;
        if(p->hasReachedDestination())
            continue;

        // Sprawdź kolizje z pionkami tego samego koloru
        if(wouldCollideWithSameColor(p, diceFace))
            continue;

        // Jeśli wszystkie warunki spełnione, dodaj do możliwych ruchów
        result.append(p);
    }

    // Jeśli nie ma możliwych ruchów, automatycznie kończymy turę
    if (result.isEmpty()) {
        qDebug() << "Brak możliwych ruchów dla gracza" ;//<< getCurrentPlayer();
        //emit noMovesAvailable(getCurrentPlayer());
    }

    return result;
}

// Dodajmy metodę obsługującą automatyczne kończenie tury
void Game::handleNoMoves() {
    qDebug() << "Automatyczne zakończenie tury gracza" ;//<< getCurrentPlayer();
    // Tu możemy dodać logikę zmiany gracza
    changeCurrentPlayer();
    // emit turnEnded(getCurrentPlayer());
}

void Game::announceVictory(PlayerColor color) {
    QString colorName;
    switch(color) {
    case PlayerColor::RED: colorName = "Czerwony"; break;
    case PlayerColor::BLUE: colorName = "Niebieski"; break;
    case PlayerColor::YELLOW: colorName = "Zolty"; break;
    case PlayerColor::GREEN: colorName = "Zielony"; break;
    }

    QMessageBox victoryBox;
    victoryBox.setWindowTitle("Gratulacje!");
    victoryBox.setText(QString("Gracz %1 wygrał grę!").arg(colorName));
    victoryBox.setIcon(QMessageBox::Information);

    // Dodaj własne style do okna dialogowego
    victoryBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: #f0f0f0;"
        "}"
        "QMessageBox QLabel {"
        "    color: #2c3e50;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        );

    victoryBox.exec();

    // Możesz też emitować sygnał o zwycięstwie
    //emit gameWon(color);
}

bool Game::checkVictoryConditions(PlayerColor color) {
    int pawnsInEndZone = 0;
    QVector<Pawn*> pawns = mBoard->getAllPawns();

    for (auto pawn : pawns) {

        if (pawn->getColor() != color) continue;
        if( pawn->getRelPosition() < 40) continue;

        QPoint pawnPos = mBoard->getPawnEndZone(color, pawn->getRelPosition());
        int distance = abs(pawnPos.x() - 5) + abs(pawnPos.y() - 5);
        // Sprawdź czy pionek jest w strefie końcowej (odległość 0-3 od środka)
        if (distance <= 4) {
            pawnsInEndZone++;
            qDebug() << "Pionkow w strefie: " << pawnsInEndZone;
        }
    }

    return pawnsInEndZone == 4;  // Zwycięstwo gdy wszystkie 4 pionki są w strefie
}

bool Game::isNearForbiddenZone(const QPoint& position, int diceValue, PlayerColor playerColor) {
    const QPoint forbiddenPoint(5, 5);

    struct DangerousPosition {
        QPoint pos;
        int maxAllowedMove;
    };

    const QVector<DangerousPosition> dangerousPositions = {
        {{4, 5}, 0}, {{5, 4}, 0}, {{6, 5}, 0}, {{5, 6}, 0},
        {{5, 3}, 1}, {{3, 5}, 1}, {{7, 5}, 1}, {{5, 7}, 1},
        {{2, 5}, 2}, {{5, 2}, 2}, {{8, 5}, 2}, {{5, 8}, 2},
        {{0, 5}, 4}, {{5, 0}, 4}, {{10, 5}, 4}, {{5, 10}, 4},
        {{6, 10}, 5}, {{10, 4}, 5}, {{4, 0}, 5}, {{0, 6}, 5},
        {{5, 1}, 3}, {{9, 5}, 3}, {{5, 9}, 3}, {{1, 5}, 3}
    };

    for (const auto& danger : dangerousPositions) {
        if (position == danger.pos) {
            if (danger.maxAllowedMove >= 4) {
                // Użyj getPawnsAt zamiast getPawnAtPosition
                QVector<Pawn*> pawnsAtPosition = mBoard->getPawnsAt(position);
                if (!pawnsAtPosition.empty()) {
                    // Sprawdź każdy pionek na tej pozycji
                    for (Pawn* pawn : pawnsAtPosition) {
                        // Jeśli znajdziemy pionek innego koloru, nie blokujemy ruchu
                        if (pawn->getColor() != playerColor) {
                            qDebug() << "Pole" << position << "ignorowane, ponieważ pionek ma inny kolor niż gracz.";
                            return false;
                        }
                    }
                }
            }

            if (diceValue > danger.maxAllowedMove) {
                qDebug() << "Pozycja" << position << "jest niebezpieczna. Maksymalny dozwolony ruch:"
                         << danger.maxAllowedMove << "Wyrzucono:" << diceValue;
                return true;
            }
            break;
        }
    }
    return false;
}

// QVector<Pawn*> Game::getPlayablePawns(int diceFace) { wersja alfa 1.0
//     if(diceFace < 1 || diceFace > 6)
//         ValueError::raise_new(QString("Invalid dice value : %1").arg(diceFace));

//     QVector<Pawn*> result;
//     QVector<Pawn*> pawns = mBoard->getAllPawns();

//     for(auto p : pawns) {
//         if(p->getColor() != getCurrentPlayer())
//             continue;
//         if(p->isAtHome() && diceFace != 6 && SIX_FOR_HOME)
//             continue;
//         if(p->hasReachedDestination())
//             continue;
//         if(p->getRelPosition() + diceFace > Pawn::DEST)
//             continue;

//         result.append(p);
//     }

//     return result;
// }

// bool Game::playMove(Pawn* pawn, int diceFace) {
//     qInfo() << "Game::playMove(Pawn*, int)";

//     //Set it to the number by which we're gonna move
//     unsigned int futureRel {Game::predictRel(pawn, diceFace)};

//     //Will the player get a turn again?
//     bool re_turn = diceFace == 6;

//     //Pawn which was hit by this one while moving
//     Pawn* toClash {nullptr};

//     //This pawn couldn't have been clashed with anybody, because it has reached destination
//     if(futureRel != Pawn::DEST) {

//         //If there is only one pawn at this new location, send it back home
//         //This should be done before moving the current pawn
//         QVector<Pawn*> pawnsThere = mBoard->getPawnsAt(
//             mBoard->getPawnCoordinates(
//                 pawn->getColor(),
//                 futureRel //Our future position
//                 )
//             );

//         if(pawnsThere.size() == 1 && pawnsThere[0]->getColor() != pawn->getColor()) {
//             toClash = pawnsThere[0];
//             re_turn = true;
//         }

//     } else {
//         //But we do get a turn again,..
//         re_turn = true;
//     }

//     //We can now move the pawn
//     pawn->changePosition(futureRel);

//     //And send the clashed one home
//     if(toClash != nullptr)
//         toClash->goBackHome();

//     return re_turn;
// }
