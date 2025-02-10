#include <Game.h>

#include <QDebug>

#include <Board.h>
#include <Pawn.h>
#include <PlayerColor.h>
#include <ValueError.h>

Game::Game(unsigned int players,const  QVector<PlayerColor>& playerColors, const QVector<QString>& playerModes)
    : playersCount(players)
    , mBoard(new Board{players, playerColors})
    , currentSequence(new QVector<PlayerColor>{})
    , currentPlayerIndex(0)
    , random(QRandomGenerator::securelySeeded())
{
    for (size_t i = 0; i < players; ++i) {
        currentSequence->push_back(playerColors[i]);
        this->playerModes.push_back(playerModes[i]);
    }
}


bool Game::isCurrentPlayerAI() const {
    bool wynik = false;
    QString cpu = "CPU";

    if(playerModes[currentPlayerIndex] == cpu)
        wynik = true;
    return wynik;
}

Game::~Game() {
    delete currentSequence;
    delete mBoard;
}

int Game::rollDice() {
    lastDiceValue = random.bounded(1, 7);
    return lastDiceValue;
}

PlayerColor Game::getCurrentPlayer() const {
    return (*currentSequence)[currentPlayerIndex];
}

Board* Game::getGameBoard() const {
    return mBoard;
}

unsigned int Game::predictRel(Pawn* pawn, unsigned int diceFace) const {
    qInfo() << "Game::predictRel(Pawn*, int)";

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
void Game::changeCurrentPlayer() {
    currentPlayerIndex = (currentPlayerIndex + 1) % currentSequence->size();
}

QVector<PlayerColor> Game::getCurrentPlayerSequence() const {
    return *currentSequence;
}

unsigned int Game::getLastDiceValue() const {
    return lastDiceValue;
}

bool Game::playerHasFinished(PlayerColor c) const {
    for(auto p : mBoard->getAllPawnsByColor(c))
        if(!p->hasReachedDestination())
            return false;

    return false;
}

bool Game::isFinished() const {
    unsigned int i = 0;
    for (auto c : *currentSequence)
        if(playerHasFinished(c))
            i++;

    return i >= playersCount - 1;
}


bool Game::playMove(Pawn* pawn, int diceFace) {
    qInfo() << "Game::playMove(Pawn*, int)";
    if (pawn->getRelPosition() + diceFace > Pawn::DEST) {
        qDebug() << "Prevented move that would exceed board limit";
        return false;
    }

    QVector<Pawn*> playablePawns = getPlayablePawns(diceFace);
    if (playablePawns.isEmpty()) {
        handleNoMoves();
        return false;
    }

    if (!playablePawns.contains(pawn)) {
        qDebug() << "The selected pawn cannot make a move";
        return false;
    }

    unsigned int futureRel = Game::predictRel(pawn, diceFace);
    bool re_turn = diceFace == 6;
    Pawn* toClash = nullptr;

    if (futureRel != Pawn::DEST) {
        QVector<Pawn*> pawnsThere = mBoard->getPawnsAt(
            mBoard->getPawnCoordinates(
                pawn->getColor(),
                futureRel
                )
            );
        if (pawnsThere.size() == 1 && pawnsThere[0]->getColor() != pawn->getColor()) {
            toClash = pawnsThere[0];
            re_turn = true;
        }
    } else {
        re_turn = true;
    }

    pawn->changePosition(futureRel);

    if (checkVictoryConditions(pawn->getColor())) {
        announceVictory(pawn->getColor());
    }

    if (toClash != nullptr) {
        toClash->goBackHome();
    }

    return re_turn;
}

bool Game::wouldCollideWithSameColor(Pawn* pawn, int diceFace) const {
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

QVector<Pawn*> Game::getPlayablePawns(int diceFace) const {
    qDebug() << "getPlayablePawns(int diceFace)";
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

        if(wouldCollideWithSameColor(p, diceFace))
            continue;

        result.append(p);
    }

    if (result.isEmpty()) {
        qDebug() << "No possible moves for the player" ;
    }

    return result;
}

void Game::handleNoMoves() {
    qDebug() << "Automatic termination of a player's turn" ;
    changeCurrentPlayer();
}

void Game::announceVictory(PlayerColor color) {
    QString colorName;
    switch(color) {
    case PlayerColor::RED: colorName = "Red"; break;
    case PlayerColor::BLUE: colorName = "Blue"; break;
    case PlayerColor::YELLOW: colorName = "Yellow"; break;
    case PlayerColor::GREEN: colorName = "Green"; break;
    }

    QMessageBox victoryBox;
    victoryBox.setWindowTitle("Congratulations!");
    victoryBox.setText(QString("Player %1 won the game!").arg(colorName));
    victoryBox.setIcon(QMessageBox::Information);

    victoryBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: #f0f0f0;"
        "}"
        "QMessageBox QLabel {"
        "    color: #2c3e50;"
        "    font-size: 45px;"
        "    font-weight: bold;"
        "}"
        );

    victoryBox.exec();
}

bool Game::checkVictoryConditions(PlayerColor color) const {
    int pawnsInEndZone = 0;
    QVector<Pawn*> pawns = mBoard->getAllPawns();

    for (auto pawn : pawns) {

        if (pawn->getColor() != color) continue;
        if( pawn->getRelPosition() < 40) continue;

        QPoint pawnPos = mBoard->getPawnEndZone(color, pawn->getRelPosition());
        int distance = abs(pawnPos.x() - 5) + abs(pawnPos.y() - 5);

        if (distance <= 4) {
            pawnsInEndZone++;
            qDebug() << "Pawns in end zone: " << pawnsInEndZone;
        }
    }

    return pawnsInEndZone == 4;
}

bool Game::isNearForbiddenZone(const QPoint& position, int diceValue, PlayerColor playerColor) const {
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
                QVector<Pawn*> pawnsAtPosition = mBoard->getPawnsAt(position);
                if (!pawnsAtPosition.empty()) {
                    for (Pawn* pawn : pawnsAtPosition) {
                        if (pawn->getColor() != playerColor) {
                            qDebug() << "Position" << position << "ignored because the pawn is a different colour from the player's.";
                            return false;
                        }
                    }
                }
            }

            if (diceValue > danger.maxAllowedMove) {
                qDebug() << "Position" << position << "is unsafe. Maximum permitted traffic:"
                         << danger.maxAllowedMove << "Dumped:" << diceValue;
                return true;
            }
            break;
        }
    }
    return false;
}
