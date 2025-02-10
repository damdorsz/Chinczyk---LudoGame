#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QString>
#include <QObject>
#include <QMainWindow>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QVector>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include <QTimer>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QKeyEvent>

#include <GameScreen.h>
#include "PlayerColor.h"
#include "GameAI.h"

class Game;
class Board;
class Pawn;
class Dice;
class SaveGameEngine;
enum class PlayerColor;

class GameWindow : public QMainWindow {
Q_OBJECT

public:
    enum GameState {ROLLING, MOVING, ANIMATING};

    static const QColor COLOR_RED;
    static const QColor COLOR_YELLOW;
    static const QColor COLOR_BLUE;
    static const QColor COLOR_GREEN;
    static const QColor BG_COLOR;
    static const QColor STROKE_COLOR;
    static const QColor COLOR_RED_LIGHT;
    static const QColor COLOR_YELLOW_LIGHT;
    static const QColor COLOR_BLUE_LIGHT;
    static const QColor COLOR_GREEN_LIGHT;

    static const int BOARD_BOUND = 5;
    static const int CELL_SIZE = 40;
    static const int GUIDER_DOT_SIZE = 4;
    static const int PAWN_SIZE = 25;
    static const int DICE_SIZE = 100;
    static const int STROKE_WIDTH = 2;
    static const int ANIMATION_DURATION = 120;
    static const int DICE_ANIMATION_DURATION = 800;
    static const int DICE_SHAKE_DISTANCE = 25;
    explicit GameWindow(unsigned int players_count, const QVector<QString> &playersNames , const QVector<PlayerColor> &playersColours, const QVector<QString> &playerModes);

    ~GameWindow();
    QString getUserName(PlayerColor color);
    void updateUi();
    bool saveGame();

signals:
    void exit();

public slots:
    void pawnClashed(Pawn *pawn);
    void pawnChosen(Pawn *p);
    void rollDiceClicked();

private slots:
    void pawnAnimationFinished(Pawn*);
    void diceAnimationFinished();
    void aboutRequested();
    void exitRequested();
    void endTurn();

private:
    unsigned int players_count {};
    Game *mGame {};
    Board *mBoard {};
    GameState state {};
    QVector<QString> mPlayersNames;
    QVector<PlayerColor> mPlayersColours;
    GameScreen *mScreen {};
    QWidget *footer {};
    QVBoxLayout *footerLayout {};
    Dice *dice;
    QLabel *hintLabel {};
    QSequentialAnimationGroup *animationGroup {};
    QParallelAnimationGroup *diceAnimationGroup {};
    bool needs_to_save {false};
    void init();
    void movePawnVisual(Pawn *p, int oldpos);
    void animateVisualMovement(Pawn* p, int startRel);
    void animateDiceRoll();
    GameAI* mGameAI;
protected:
    void closeEvent(QCloseEvent*);
    void keyPressEvent(QKeyEvent *event) override;
};
#endif //GameWindow.h
