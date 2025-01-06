#ifndef BOARD_H
#define BOARD_H


#include <QWidget>
#include <QVector>
#include <QString>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>

class Board : public QWidget
{
    Q_OBJECT

public:
    explicit Board(QWidget *parent = nullptr);

    void initializeGame(const QVector<QString> &playerTypes); // "Human" lub "CPU"
    int rollDice();                                            // Rzut kostką
    bool movePawn(int playerIndex, int pawnIndex, int diceRoll); // Ruch pionka
    void updateBoard();                                         // Odświeżenie planszy

    QVector<QVector<int>> getPawnPositions() const;            // Pozycje pionków
    int getCurrentPlayer() const;                              // Aktualny gracz
    QString getPlayerType(int playerIndex) const;              // Typ gracza (Human/CPU)
    void setPlayerSettings(const QVector<QString>& colors, const QVector<QString>& modes);
signals:
    void gameOver(int playerIndex); // Sygnał zakończenia gry
    void gameUpdated(); // Sygnał zaktualizowania gry
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int currentPlayer;
    QVector<QVector<int>> pawnPositions; // Pozycje pionków [gracz][pionek]
    QVector<QString> playerTypes;        // Typy graczy: "Human" lub "CPU"
    QVector<bool> hasPlayerFinished;     // Czy gracz zakończył grę
    QVector<QPushButton*> buttons;      // Przyciski do reprezentacji pól
    QVector<QString> playerColors;
    QVector<QString> playerModes;
    void drawBoard(QPainter &painter); // Funkcja rysująca planszę
    void drawPlayerPawns(QPainter &painter); // Funkcja rysująca pionki graczy
    void onRollButtonClicked();

};

#endif // BOARD_H
