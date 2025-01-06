#include "Board.h"
#include <QRandomGenerator>
#include <QMessageBox>
#include <QPushButton>

// Konstruktor
Board::Board(QWidget *parent)
    : QWidget(parent),
    currentPlayer(0) {
    // Inicjalizacja stanu gry
    playerTypes = QVector<QString>();
    hasPlayerFinished = QVector<bool>();
    pawnPositions = QVector<QVector<int>>();
    playerColors = QVector<QString>();
    playerModes = QVector<QString>();
    setFixedSize(400, 400);

    // Dodanie przycisku do rzutu kością
    QPushButton *rollButton = new QPushButton("Rzuć kostką", this);
    rollButton->setGeometry(10, 10, 100, 30); // Ustawienie pozycji i rozmiaru przycisku
    connect(rollButton, &QPushButton::clicked, this, &Board::onRollButtonClicked);
}

// Inicjalizacja gry
void Board::initializeGame(const QVector<QString> &playerTypes) {
    this->playerTypes = playerTypes;
    currentPlayer = 0;
    hasPlayerFinished.resize(playerTypes.size());
    std::fill(hasPlayerFinished.begin(), hasPlayerFinished.end(), false);

    pawnPositions.resize(playerTypes.size());
    for (auto &positions : pawnPositions) {
        positions.resize(4); // Zakładamy 4 pionki na gracza
        std::fill(positions.begin(), positions.end(), -1); // Pionki na startowych pozycjach
    }
    updateBoard();
}

// Rzut kostką
int Board::rollDice() {
    return QRandomGenerator::global()->bounded(1, 7); // Liczby od 1 do 6
}

// Ruch pionka
bool Board::movePawn(int playerIndex, int pawnIndex, int diceRoll) {
    if (playerIndex < 0 || playerIndex >= pawnPositions.size() ||
        pawnIndex < 0 || pawnIndex >= pawnPositions[playerIndex].size()) {
        return false; // Nieprawidłowe dane
    }

    int &pawnPosition = pawnPositions[playerIndex][pawnIndex];
    if (pawnPosition == -1 && diceRoll == 6) {
        // Wprowadzenie pionka na planszę
        pawnPosition = 0;
    } else if (pawnPosition != -1) {
        // Ruch pionka na planszy
        pawnPosition += diceRoll;
        if (pawnPosition >= 40) { // Zakładamy planszę o 40 polach
            hasPlayerFinished[playerIndex] = true;
            emit gameOver(playerIndex); // Gra zakończona dla gracza
        }
    }
    emit gameUpdated();
    updateBoard();
    return true;
}
// Obsługa rzutu kością
void Board::onRollButtonClicked() {
    int diceRoll = rollDice(); // Rzut kostką
    // Logika do przesuwania pionka
    movePawn(currentPlayer, 0, diceRoll); // Przykład: przesuwamy pierwszy pionek gracza
    currentPlayer = (currentPlayer + 1) % playerTypes.size(); // Przełącz na następnego gracza
}

// Odświeżenie planszy
void Board::updateBoard() {
    repaint(); // Wywołanie odświeżenia planszy
}

// Funkcja malująca planszę
void Board::paintEvent(QPaintEvent *event) {

    Q_UNUSED(event);
    QPainter painter(this);
    drawBoard(painter);
    drawPlayerPawns(painter);
}

// Rysowanie planszy
void Board::drawBoard(QPainter &painter) {
    // Prosta plansza: rysujemy siatkę
    painter.setPen(Qt::black);
    int size = std::min(width(), height());
    int cellSize = size / 10;
    for (int i = 0; i <= 10; ++i) {
        painter.drawLine(i * cellSize, 0, i * cellSize, size);
        painter.drawLine(0, i * cellSize, size, i * cellSize);
    }
}

// Rysowanie pionków
void Board::drawPlayerPawns(QPainter &painter) {
    int size = std::min(width(), height());
    int cellSize = size / 10;

    for (int i = 0; i < pawnPositions.size(); ++i) {
        painter.setBrush(QColor(playerColors[i % playerColors.size()]));
        for (int j = 0; j < pawnPositions[i].size(); ++j) {
            if (pawnPositions[i][j] != -1) {
                int x = (pawnPositions[i][j] % 10) * cellSize;
                int y = (pawnPositions[i][j] / 10) * cellSize;
                painter.drawEllipse(x + cellSize / 4, y + cellSize / 4, cellSize / 2, cellSize / 2);
            }
        }
    }
}

// Pobranie pozycji pionków
QVector<QVector<int>> Board::getPawnPositions() const {
    return pawnPositions;
}

// Pobranie aktualnego gracza
int Board::getCurrentPlayer() const {
    return currentPlayer;
}

// Pobranie typu gracza
QString Board::getPlayerType(int playerIndex) const {
    if (playerIndex >= 0 && playerIndex < playerTypes.size()) {
        return playerTypes[playerIndex];
    }
    return QString();
}

// Ustawienia graczy
void Board::setPlayerSettings(const QVector<QString>& colors, const QVector<QString>& modes) {
    playerColors = colors;
    playerModes = modes;
}


